#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "udp.h"


int main(int argc, char *argv[]) {
	/*
	* [Description]	:
	*	- server에게 데이터를 전달하고 확인 메세지를 받음
	* [Input]		:
	*	MsgType msg // 서버로부터 받을 확인 메세지
	* [Output]		:
	*	MsgType msg // 서버에게 보낼 데이터
	* [Calls]		:
	*	None
	* [특기사항]	:
	*	- SERY_HOST_ADDR에서 udp 서버가 돌아가고 있어야 함
	*/
	int					sockfd, n, peerAddrLen;
	struct sockaddr_in	servAddr, peerAddr;
	MsgType				msg;

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  { // socket 을 udp 로 만듬
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); //4바이트 ip address 를 네트워크 바이트로 바꿈 
	servAddr.sin_port = htons(SERV_UDP_PORT);
	// bind 를 안했기 때문에 운영체제가 알아서 bind 를 함 ip는 내 호스트 address 포트는 알아서 빈 공간에 
	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if (sendto(sockfd, (char *)&msg, sizeof(msg), 
			0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)  {
		perror("sendto");
		exit(1);
	}
	printf("Sent a request.....");
		
	peerAddrLen = sizeof(peerAddr);
	if ((n = recvfrom(sockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0)  {
		perror("recvfrom");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);
	return 0;
}

