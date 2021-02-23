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

char buf[128] = {0};

int main(int argc, const char *argv[])
{
	int fd;
	if((fd = open("./hello",O_RDWR))<0){
		perror("open error");
		return -1;
	}

	read(fd,buf,sizeof(buf));
	write(fd,buf,sizeof(buf));

	close(fd);
	return 0;
}
