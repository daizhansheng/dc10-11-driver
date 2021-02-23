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
	printf("input string > ");
	fgets(buf,sizeof(buf),stdin);
	buf[strlen(buf)-1] = '\0';

	write(fd,buf,strlen(buf));
	//将用户空间的数据写入到内核空间

	memset(buf,0,sizeof(buf));

	read(fd,buf,sizeof(buf));
	//将内核空间的数据读取到用户空间
	
	printf("user:%s\n",buf);

	close(fd);
	return 0;
}
