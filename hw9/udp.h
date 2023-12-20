#define MY_ID			14

#define	SERV_UDP_PORT	(8000 + MY_ID)
#define	SERV_HOST_ADDR	"163.180.140.216"

#define	MSG_REQUEST		1
#define	MSG_REPLY		2


typedef struct  {
	int		type;
	char	data[128];
}
	MsgType;

