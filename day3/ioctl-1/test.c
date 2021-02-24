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
#include <sys/ioctl.h>

#include "head.h"
char buf[128] = "i am user array data......";
int main(int argc, const char *argv[])
{
	int fd;
	int data = 1234;
	if((fd = open("/dev/myled",O_RDWR))<0){
		perror("open error");
		return -1;
	}
	ioctl(fd,ACCESS_WRITE_INT_DATA,&data);
	ioctl(fd,ACCESS_READ_INT_DATA,&data);
	printf("user:%d\n",data);
	ioctl(fd,ACCESS_WRITE_CHAR_ARRAY,buf);
	while(1){
		ioctl(fd,RGB_LED_RED_ON);
		sleep(1);
		ioctl(fd,RGB_LED_RED_OFF);
		sleep(1);
	}
	close(fd);
	return 0;
}
