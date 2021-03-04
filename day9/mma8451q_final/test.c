/*******************************************************************************
 * @filename    test.c 
 * @fcopyright  Copyright 2021 hqyj CO. Ltd.
 * @fauthor     [daizhansheng] [daizs_jt@hqyj.com]
 * @fversion    1.0
 * @fhistory    [2021-02-23] Create file.
 ******************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


unsigned char buf[6] = {0};
int x,y,z;
int main(int argc, const char *argv[])
{
	int fd;
	if((fd = open("/dev/mma8451q",O_RDWR))<0){
		perror("open error");
		return -1;
	}
	while(1){
		read(fd,buf,sizeof(buf));
		x = buf[0];
		x = x<<6;
		x |= buf[1]>>2;
		
		y = buf[2];
		y = y<<6;
		y |= buf[3]>>2;
		
		z = buf[4];
		z = z<<6;
		z |= buf[5]>>2;
		
		printf("x = %d,y = %d,z = %d\n",x,y,z);
		sleep(1);
	}
	close(fd);
	return 0;
}
