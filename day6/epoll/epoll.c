#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

char buf[128] = {0};
int main(int argc, const char *argv[])
{
	int epfd,i,fd,ret;
	struct epoll_event event;
	struct epoll_event revents[10];
	epfd = epoll_create(10);
	if(epfd == -1){
		perror("epoll create error");
		return -1;
	}

	for(i=1;i<argc;i++){
		fd = open(argv[i],O_RDWR);
		if(fd == -1){
			perror("open error");
			return -1;
		}
		event.events = EPOLLIN;
		event.data.fd = fd;
		ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
		if(ret){
			perror("epoll ctl");
			return -1;
		}
	}

	while(1){
		ret = epoll_wait(epfd,revents,10,-1);
		if(ret == -1){
			perror("epoll wait error");
			return -1;
		}

		for(i=0;i<ret;i++){
			if(revents[i].events & EPOLLIN){
				memset(buf,0,sizeof(buf));
				read(revents[i].data.fd,buf,sizeof(buf));
				printf("fd = %d,data = %s\n",revents[i].data.fd,
						buf);
			}

		}


	}

	for(i=0;i<=getdtablesize();i++){
		close(i);
	}
	return 0;
}
