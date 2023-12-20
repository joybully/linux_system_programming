#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcp.h"


int main(int argc, char *argv[]) {
	/*
	* [Description]	:
	*	- tcp server에게 데이터를 전달하고 확인 메세지를 받음
	* [Input]		:
	*	char* argv[1] // IPAddress
	* [Output]		:
	*	MsgType msg // 서버에게 보낼 데이터
	* [Calls]		:
	*	None
	* [특기사항]	:
	*	- SERY_HOST_ADDR에서 udp 서버가 돌아가고 있어야 함
	*	- DNS를 사용
	*/
	int					sockfd, n;
	struct sockaddr_in	servAddr;
	MsgType				msg;
	struct hostent		*hp;

	if (argc != 2)  {
		fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
		exit(1);
	}

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (isdigit(argv[1][0]))  { // 첫번째 어드레스가 digit 이면 도티드 즉 숫자 주소
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else  { // 숫자가 아닐경우 
		if ((hp = gethostbyname(argv[1])) == NULL)  { // host name이 숫자가 아닐때 바꿔줌 그 주소에 포인터를 반환해줌 
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}

	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {	// 서버의 소켓 주소 설정
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if (write(sockfd, (char *)&msg, sizeof(msg)) < 0)  {
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");
		
	if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);
	return 0;
}
