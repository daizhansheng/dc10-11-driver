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
	int ret,i;
	int fd1,fd2;
	fd_set rfds;
	if((fd1 = open("/dev/mycdev0",O_RDWR))<0){
		perror("open error");
		return -1;
	}
	if((fd2 = open("/dev/input/mouse1",O_RDWR))<0){
		perror("open error");
		return -1;
	}
	while(1){

		FD_ZERO(&rfds);
		FD_SET(fd1,&rfds);
		FD_SET(fd2,&rfds);
		ret = select(fd2+1,&rfds,NULL,NULL,NULL);
		if(ret < 0){
			perror("select error");
			return -1;
		}

		for(i=fd1;i<=fd2;i++){
			if(FD_ISSET(i,&rfds)){
				read(i,buf,sizeof(buf));
				if(i==fd1)
					printf("mydriver:%s\n",buf);
				else if(i==fd2)
					printf("mouse1:%s\n",buf);
			}

		}

	}
	close(fd1);
	close(fd2);
	return 0;
}
