#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "select.h"


int	TcpSockfd;
int	UdpSockfd;

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
	close(TcpSockfd);
	close(UdpSockfd);

	printf("\nServer daemon exit.....\n");
	exit(0);
}

void MakeTcpSocket() {
	/*
	* [Function Name]: void MakeTcpSocket()
	* [Description]	 :
	*	- TCP 소켓을 생성
	* [Input]		 :
	*	None
	* [Output]		 :
	*	int TcpSockfd	// 생성된 tcp 소켓의 디스크립터
	* [Call By]		 :
	*	int main()
	* [Calls]		 :
	*	None
	* [Given]		 :
	*	None
	* [Returns]		 :
	*	None
	*/
	struct sockaddr_in	servAddr;

	if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { // 소켓 생성
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // 주소를 0으로 초기화
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT); // 서버에 대한 설정 TCP에 맞게

	if (bind(TcpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { // 소켓 주소 할당
		perror("bind");
		exit(1);
	}

	listen(TcpSockfd, 5); // 응답을 기다림 
}

void MakeUdpSocket() {
	/*
	* [Function Name]: void MakeUdpSocket()
	* [Description]	 :
	*	- UDP 소켓을 생성
	* [Input]		 :
	*	None
	* [Output]		 :
	*	int UdpSockfd	// 생성된 udp 소켓의 디스크립터
	* [Call By]		 :
	*	int main()
	* [Calls]		 :
	*	None
	* [Given]		 :
	*	None
	* [Returns]		 :
	*	None
	*/
	struct sockaddr_in	servAddr;

	if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_UDP_PORT); // UDP 세팅

	if (bind(UdpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

}

void ProcessTcpRequest() {
	/*
	* [Function Name]: void ProcessTcpRequest()
	* [Description]	 :
	*	- TCP 리퀘스트 처리
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
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr); // 클라이언트 주소 길이
	newSockfd = accept(TcpSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen); // 길이만큼 새로운 소켓을 생성해줌 
	if (newSockfd < 0)  {
		perror("accept");
		exit(1);
	}
		
	if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}
	printf("Received TCP request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
		perror("write");
		exit(1);
	}
	printf("Replied.\n");

	close(newSockfd);
}

void ProcessUdpRequest() {
	/*
	* [Function Name]: void ProcessTcpRequest()
	* [Description]	 :
	*	- UDP 리퀘스트 처리
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
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	if ((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg), 
				0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
		perror("recvfrom");
		exit(1);
	}
	printf("Received UDP request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if (sendto(UdpSockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");
}

int main(int argc, char *argv[]) {
	/*
	* [Program Name]: Assignment 10-9
	* [Description]	:
	*	- client로부터 데이터를 받고 확인 메세지를 보냄
	* [Input]		:
	*	None
	* [Output]		:
	*	None
	* [Calls]		:
	*	void CloseServer()
	*	void MakeTcpSocket()
	*	void MakeUdpSocket()
	*	void ProcessTcpRequest()
	*	void ProcessUdpRequest()
	* [특기사항]	:
	*	- 서버는 keyboard interrupt 이전까지 계속해서 작동해야 함
	*	- 각 요청마다 스레드를 생성하여 처리
	*	- 요청은 하나씩 따로 처리
	*/
	fd_set	fdvar;
	int		count;
	pthread_t tid1, tid2; // 쓰레드 생성 변수

	
	signal(SIGINT, CloseServer);

	MakeTcpSocket(); // TCP socket 생성
	MakeUdpSocket(); // UDP

	printf("Server daemon started.....\n");

	if (pthread_create(&tid1, NULL, (void *)ProcessTcpRequest,NULL) < 0)  {
          perror("pthread_create");
          exit(1);
    }
	if (pthread_create(&tid2, NULL, (void *)ProcessUdpRequest, NULL) < 0)  {
            perror("pthread_create");
            exit(1);
     }
	
	while (1)  {
		FD_ZERO(&fdvar);
		FD_SET(TcpSockfd, &fdvar);
		FD_SET(UdpSockfd, &fdvar);
		if ((count = select(10, &fdvar, (fd_set *)NULL, (fd_set *)NULL, 
			(struct timeval *)NULL)) < 0)  {
			perror("select");
			exit(1);
		}
		while (count--)  { // 입력이 들어올때 그에 맞는 쓰레드 실행 
			if (FD_ISSET(TcpSockfd, &fdvar))  {
				if(pthread_join(tid1,NULL)<0){
					perror("pthread_join");
					exit(1);
				}
			}
			else if (FD_ISSET(UdpSockfd, &fdvar))  {
				if(pthread_join(tid2,NULL)<0){
                    perror("pthread_join");
                    exit(1);
 				}	
			}
		}
	}

	return 0;
}

