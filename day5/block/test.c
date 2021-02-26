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

char buf[128] = "i am test wait event code....";

int main(int argc, const char *argv[])
{
	int fd;
	pid_t pid;
	if((fd = open("/dev/mycdev1",O_RDWR))<0){
		perror("open error");
		return -1;
	}
	pid = fork();
	if(pid < 0){
		perror("fork error");
		return -1;
	}else if(pid == 0){
		while(1){
			memset(buf,0,sizeof(buf));
			read(fd,buf,sizeof(buf));
			printf("read = %s\n",buf);
		}
	}else{
		while(1){
			sleep(2);
			write(fd,buf,sizeof(buf));
		}
	
	}

	close(fd);
	return 0;
}
