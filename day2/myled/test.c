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
#include <string.h>
char buf[128] = {0};

int main(int argc, const char *argv[])
{
	int fd;
	if((fd = open("./hello",O_RDWR))<0){
		perror("open error");
		return -1;
	}

	while(1){
		scanf("%d,%d",&buf[0],&buf[1]);

		write(fd,buf,sizeof(buf));
		//将用户空间的数据写入到内核空间

	}
	close(fd);
	return 0;
}
