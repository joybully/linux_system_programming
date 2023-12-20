#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"


int	Sockfd;

void CloseServer() {
	/*
	* [Function Name]: void CloseServer()
	* [Description]	 :
	*	- keybord interrupt가 발생하면 socket을 닫는 signal handler
	* [Input]		 :
	*	None
	* [Output]		 :
	*	None
	* [Call By]		 :
	*	int main()
	* [Calls]		 :
	*	None
	* [Given]		 :
	*	None
	* [Returns]		 :
	*	None
	*/
	close(Sockfd);
	printf("\nTCP Server exit.....\n");

	exit(0);
}


int main(int argc, char *argv[]) {
	/*
	* [Description]	:
	*	- client에게 데이터를 받고 확인 메세지를 전달함
	* [Input]		:
	*	MsgType msg // 연결된 client로부터 받아온 메세지
	* [Output]		:
	*	MsgType msg // client에게 보낼 확인 메세지
	* [Calls]		:
	*	void CloseServer()
	* [특기사항]	:
	*	- 서버는 keyboard interrupt가 발생하기 이전까지 계속 작동함
	*	- client와의 통신은 개별 프로세스를 만들어 수행
	*/
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr; // socket address (서버와 클라이언트 각각)
	MsgType				msg; // 전달될 메세지 구조체
	pid_t pid; // 프로세스 생성위한 변수


	signal(SIGINT, CloseServer); // 서버 종료 핸들러 

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { // 전역변수로 만드는 이유는 signal handler 때문
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // 처음 바이트를 0으로 채움 
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 보통 서버에서는 모든 포트를 받아야 하기 때문에 INADDR_ANY 사용 
	servAddr.sin_port = htons(SERV_TCP_PORT); // 내 호스트 크기만큼 (처리가능한 크기만큼) 바꿔줘야함 
	// 소켓에 주소를 할당하는 것이 bind
	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { // 내가 사용할 소켓 프로세스를 입력해주는 것 , 구조체의 크기만큼 bind 시켜줌 , 
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen); // 사용자에 connection 을 기다림 , ip4를 사용하기 때문에 그 만큼에 소켓 길이만큼 입력해둠 
		if (newSockfd < 0)  { 
			perror("accept");
			exit(1);
		}
		
		pid = fork(); // 자식프로세스를 만들어줌

		if(pid == 0){ // 자식프로세스에서 실행 
		
			if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  { // client 한테 데이터를 받을때는 newSockfd 이용 
				perror("read");
				exit(1);
			}
			printf("Received request: %s.....", msg.data);

			msg.type = MSG_REPLY;
			sprintf(msg.data, "This is a reply from %d.", getpid());
			if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  { //client한테 잘 받았다고 전달 용 
				perror("write");
				exit(1);
			}
			printf("Replied.\n");

			usleep(10000); // client 가 받는 시간을 위해서 sleep 
			close(newSockfd); // 바로 종료되기 때문에 connection 을 해지함 
		}

		else{
			close(newSockfd);
		}	
	}
	return 0;
}

