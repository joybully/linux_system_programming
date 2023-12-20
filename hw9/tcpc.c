#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"


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
	*	- SERY_HOST_ADDR에서 tcp 서버가 돌아가고 있어야 함
	*/
	int					sockfd, n;
	struct sockaddr_in	servAddr;
	MsgType				msg;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { // 소켓을 하나 만들어둠 
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // string 을 ip4버전 형식으로 바꿔줌 
	servAddr.sin_port = htons(SERV_TCP_PORT);
	// bind 를 안해줌 
	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { // 0으로 초기화 해 둔 다음에 connect 를 해줌 
		perror("connect");
		exit(1);
	} // 성공하면 server 와 같이 연결됨 

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if (write(sockfd, (char *)&msg, sizeof(msg)) < 0)  { // 구조체의 크기만큼 데이터를 보내주는 것 
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");
		
	if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0)  { // 서버가 나한테 응답하는 것을 받아옴 
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);
	return 0;
}

