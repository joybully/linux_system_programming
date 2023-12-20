#define MY_ID			14
#define	SHM_KEY			(0x9000 + MY_ID)
#define	SHM_MODE		(SHM_R | SHM_W | IPC_CREAT)

#define	EMPTY_SEM_KEY	(0x5000 + MY_ID)
#define	FULL_SEM_KEY	(0x6000 + MY_ID)
#define	MUTEX_SEM_KEY	(0x7000 + MY_ID)

#define	NLOOPS			20

#define	MAX_BUF			2 // 큐에 크기는 2로 해놨음 꽉 차는 것을 조절하기 위해서 

typedef struct {
	int		data;
} ItemType;

typedef struct BoundedBufferType {
	ItemType	buf[MAX_BUF];
	int			in;
	int			out;
	int			counter;
} BoundedBufferType;

#define	SHM_SIZE	sizeof(BoundedBufferType)

