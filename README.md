# RESP [![Build Status](https://travis-ci.org/interma/RESP.svg?branch=master)](https://travis-ci.org/interma/RESP)

A RESP(REdis Serialization Protocol) C Lib, it make tcp server parse resp request and send resp response easily.

So you can use the existing redis-cli(or other redis client lib) communicate with your tcp server conveniently.

##REdis Serialization Protocol

See: http://redis.io/topics/protocol, the key points:

>The way RESP is used in Redis as a request-response protocol is the following:

    Request: Clients send commands to a Redis server as a RESP Array of Bulk Strings.
    Response: The server replies with one of the RESP types according to the command implementation.

##Example##
###simple example###
```c
#include "resp.h"
int main() {
	int ret = 0;
    RespRequest *req = create_request(1024);
    RespResponse *res = create_response(128);
    
    char buf[]="*2\r\n$4\r\nLLEN\r\n$6\r\nmylist\r\n";
    ret = decode_request(req, buf, sizeof(buf)-1);
    char buf1[] = "foobar";
    ret = encode_response_string(res, buf1, strlen(buf1));
    
    destroy_request(req);
    destroy_response(res);
    return 0;
}
```
###server example###
code segment, the full code pls see: https://github.com/interma/RESP/blob/master/src/sample_svr.c
```c
{...
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
...}
```
redis-cli visit
```c
interma@debian:~/package/redis-3.2.0/src$ ./redis-cli -p 1220
127.0.0.1:1220> add 1 2 3 5
(integer) 11
127.0.0.1:1220> get xxx
(error) ERR unknown command
```
##other##
find some bugs, welcome contact me or propose a pull request.
