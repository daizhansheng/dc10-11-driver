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
int main(int argc, const char *argv[])
{
	int fd;
	if((fd = open("/dev/myled",O_RDWR))<0){
		perror("open error");
		return -1;
	}

	while(1){
		ioctl(fd,RGB_LED_RED_ON);
		sleep(1);
		ioctl(fd,RGB_LED_RED_OFF);
		sleep(1);
	}
	close(fd);
	return 0;
}
