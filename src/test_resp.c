/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/17/16 17:05:19 HKT
*/

#include <stdio.h>
#include "resp.h"

int main() {
	int ret = 0;
	RespRequest *req = create_request(1024);
	
	char buf[]="*2\r\n$4\r\nLLEN\r\n$6\r\nmylist\r\n";
	ret = decode_request(req, buf, sizeof(buf));
	if (ret == 0 && req->status == OK) {
		printf("decode ok\n");
	}

	destroy_request(req);

	RespResponse *res = create_response(128);
	ret = encode_response_simplestring(res, 0, "OK");
	if (ret == 0) {
		printf("encode ok\n");
	}

	destroy_response(res);

	printf("all test ok\n");
	return 0;
}

