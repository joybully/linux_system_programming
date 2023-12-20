#define MY_ID			14

#define	SERV_TCP_PORT	(7000 + MY_ID)
// celinux14 ip address
#define SERV_HOST_ADDR  "163.180.140.216"

#define	MSG_REQUEST		1
#define	MSG_REPLY		2

typedef struct {
	int		type;
	char	data[128];
} MsgType;
