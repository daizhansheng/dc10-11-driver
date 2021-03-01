#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int fd;
char buf[128] = {0};

void signal_handle_func(int signo)
{
	if(signo == SIGIO){
		memset(buf,0,sizeof(buf));
		read(fd,buf,sizeof(buf));
		printf("fasync data = %s\n",buf);
	}
}

int main(int argc, const char *argv[])
{
	if((fd = open("/dev/mycdev0",O_RDWR))<0){
		perror("open");
		return -1;
	}

	signal(SIGIO,signal_handle_func);

	fcntl(fd,F_SETFL,fcntl(fd,F_GETFL)|FASYNC);

	fcntl(fd,F_SETOWN,getpid());

	while(1){
		sleep(1);
	}

	close(fd);
	return 0;
}
