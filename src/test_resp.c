/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/17/16 17:05:19 HKT
*/

#include <stdio.h>
#include "resp.h"

int main() {
	RespRequest *req = create_request(1024);
	destroy_request(req);
	printf("all test ok\n");
	return 0;
}
