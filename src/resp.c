/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/17/16 17:04:52 HKT
*/

#include <string.h>
#include <stdlib.h>
#include "resp.h"

RespRequest *create_request(uint32_t hint_buf_size) {
	RespRequest* req = (RespRequest*)malloc(sizeof(RespRequest));	
	if (req == NULL)
		return NULL;
	memset(req, 0 , sizeof(RespRequest));
	
	req->status = INIT;
	req->buf_size = hint_buf_size;
	req->buf = (char *)malloc(hint_buf_size);
	if (req->buf == NULL) {
		free(req);
		return NULL;
	}

	return req;

}
void destroy_request(RespRequest *request) {
	if (request == NULL)
		return;

	if (request->buf != NULL)
		free(request->buf);
	if (request->argv != NULL)
		free(request->argv);
	free(request);	
}







