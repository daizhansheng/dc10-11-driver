#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#define PATH "/dev/mcp2515_dev"

int main(int argc, const char *argv[])
{
	int fd ,i ;
	unsigned char wr_buf[14] = {0x0b,0x23,0x08,0x11,0x00,0x08,0x11,0x22,
		0x33,0x66,0x55,0x66,0x77,0x88};
	unsigned char rd_buf[14] = {0};
	fd = open(PATH,O_RDWR);
	if(fd < 0){
		perror("open");
	}
	if(write(fd,&wr_buf,sizeof(wr_buf)) < 0){
		perror("write");
	}
	
	if(read(fd,&rd_buf,sizeof(rd_buf)) < 0){
		perror("read");
	}
	for(i = 0; i < sizeof(rd_buf)/sizeof(rd_buf[0]) ; i++)
		printf("rd_buf[%d] :%x \n",i,rd_buf[i]);
	close(fd);
	return 0;
}
