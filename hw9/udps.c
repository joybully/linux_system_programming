#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "udp.h"

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
	printf("\nUDP Server exit.....\n");

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
	*	- udp 서버 형식을 따름(connectionless)
	*/
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;
	MsgType				msg;

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  { // udp 에서는 DGRAM 을 사용함 
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // server에는 INADDR_ANY 는 어디로 들어오든 처리하겠다는 의미 
	servAddr.sin_port = htons(SERV_UDP_PORT); // 포트 넘버 설정 

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { // socket 은 bind 를 해줘야 하므로 
		perror("bind");
		exit(1);
	}

	printf("UDP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	printf("%ld\n",sizeof(msg));
	while (1)  {
		if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg), 
					0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  { // UDP 는 connect 가 필요없기 때문에 바로 기다림 
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);
		// 여기에서 어디ip address 에서 왔는지 확인하려면 cliAddr을 까보면됨 
		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (sendto(Sockfd, (char *)&msg, sizeof(msg),
					0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  { 
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
	return 0;
}
