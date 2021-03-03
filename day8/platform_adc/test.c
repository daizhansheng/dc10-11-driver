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


unsigned int data;
float r_v;
int main(int argc, const char *argv[])
{
	int fd;
	if((fd = open("/dev/myadc",O_RDWR))<0){
		perror("open error");
		return -1;
	}
	while(1){
		read(fd,&data,sizeof(data));
		
		r_v = data/4096.0 *1.8;
		printf("V = %.2f\n",r_v);
		sleep(1);
	}
	close(fd);
	return 0;
}
