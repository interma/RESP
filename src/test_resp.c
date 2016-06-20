/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/17/16 17:05:19 HKT
*/

#include <stdio.h>
#include <assert.h>
#include "resp.h"

void request_assert(int val, char *str, RespRequest *r) {
	if (val == 0) {
		printf("%s\n",str);
		print_request(r);
		assert(0);
	}
}

int main() {
	int ret = 0;
	RespRequest *req = create_request(1024);
	
	//==request test==
	{
	char buf[]="*2\r\n$4\r\nLLEN\r\n$6\r\nmylist\r\n";
	ret = decode_request(req, buf, sizeof(buf)-1);
	request_assert( ret == 0 && req->state == OK, "normal", req);
	//print_request(req);
	}
	{
	reset_request(req);
	char buf[]="*2\r\n$4\r\nLLEN\r\n$6\r\nmylist\r\nxxx";
	ret = decode_request(req, buf, sizeof(buf)-1);
	request_assert( ret == 0 && req->state == PART_OK, "part", req);
	}
	{
	reset_request(req);
	char buf[]="*2\r\n$4\r\nLLEN\r";
	ret = decode_request(req, buf, sizeof(buf)-1);
	request_assert( ret == 0 && req->state == NOT_YET, "not yet", req);
	}
	{
	reset_request(req);
	char buf[]="*2\r\n$4\r\nLLEN\r";
	char buf2[]="\n$6\r\nmylist\r\n";
	ret = decode_request(req, buf, sizeof(buf)-1);
	ret = decode_request(req, buf2, sizeof(buf2)-1);
	request_assert( ret == 0 && req->state == OK, "multi buffer", req);
	}
	{
	reset_request(req);
	char buf[]="*2\r\n$4\r\nL";
	char buf2[]="LEN\r\n$6\r\nmylist\r\n";
	ret = decode_request(req, buf, sizeof(buf)-1);
	ret = decode_request(req, buf2, sizeof(buf2)-1);
	request_assert( ret == 0 && req->state == OK, "multi buffer2", req);
	}

	destroy_request(req);
	

	//==response test==
	RespResponse *res = create_response(128);
	ret = encode_response_simplestring(res, 0, "OK");
	if (ret == 0) {
	}
	destroy_response(res);

	printf("all test ok\n");
	return 0;
}

