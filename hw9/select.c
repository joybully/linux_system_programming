#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "select.h"

int	TcpSockfd;
int	UdpSockfd;

void CloseServer(){
	/*
	* [Function Name]: void CloseServer()
	* [Description]	 :
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
	* [Input]		 :
	*	None
	* [Output]		 :
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

	if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(TcpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(TcpSockfd, 5);
}

void MakeUdpSocket() {
	/*
	* [Function Name]: void MakeUdpSocket()
	* [Description]	 :
	* [Input]		 :
	*	None
	* [Output]		 :
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
	servAddr.sin_port = htons(SERV_UDP_PORT);

	if (bind(UdpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

}

void ProcessTcpRequest() {
	/*
	* [Function Name]: void ProcessTcpRequest()
	* [Description]	 :
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

	cliAddrLen = sizeof(cliAddr);
	newSockfd = accept(TcpSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
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
	* [Program Name]: Assignment 10-8
	* [Description]	:
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
	*/
	fd_set	fdvar;
	int		count;

	signal(SIGINT, CloseServer);

	MakeTcpSocket();
	MakeUdpSocket();

	printf("Server daemon started.....\n");

	while (1)  {
		FD_ZERO(&fdvar);		
		FD_SET(TcpSockfd, &fdvar);
		FD_SET(UdpSockfd, &fdvar);
		if ((count = select(10, &fdvar, (fd_set *)NULL, (fd_set *)NULL,
			(struct timeval *)NULL)) < 0)  {
			perror("select");
			exit(1);
		}
		while (count--)  {
			if (FD_ISSET(TcpSockfd, &fdvar))  {
				ProcessTcpRequest();
			}
			else if (FD_ISSET(UdpSockfd, &fdvar))  {
				ProcessUdpRequest();
			}
		}
	}

	return 0;
}

