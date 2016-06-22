/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/22/16 14:27:34 HKT
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/tcp.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

#include "resp.h"

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
	int port = 1220;
	if (argc > 1) 
		port = atoi(argv[1]);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	int flags = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&flags, sizeof(flags));
	setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind()");
		if(fd > 0) close(fd);
		exit(1);
	}

	if (listen(fd, 1024) < 0) {
		perror("listen()");
		exit(1);
	}

	RespRequest *req = create_request(BUF_SIZE);
	RespResponse *res = create_response(BUF_SIZE);
	while (1) {
		reset_request(req);
		reset_response(res);	

		socklen_t addr_len = sizeof(addr);
		int cfd = accept(fd, (struct sockaddr*)&addr, &addr_len);
		if (cfd < 0) {
			perror("accept()");
			continue;
		}
		char buf[1024];
		int rnum = read(cfd,buf,sizeof(buf));
		
		if (decode_request(req, buf, rnum) == 0 && 
			(req->state == OK || req->state == PART_OK) ){
			if (req->argc > 1 && strncmp(request_argv(req,0), "add", 3) == 0) {
				int total = 0;
				for (int i = 1; i < req->argc; i++)
					total += atoi(request_argv(req,i));
				encode_response_integer(res, total);
			}
			else {
				encode_response_status(res,0,"ERR unknown command");
			}
		}
		else {
			encode_response_status(res,0,"ERR decode command fail");
		}
		write(cfd, res->buf, res->used_size);
		close(cfd);
	}
	destroy_request(req);
	destroy_response(res);

	close(fd);
	return 0;
}
