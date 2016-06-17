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

int decode_request(RespRequest *request, const char *buf, uint32_t buf_len) {
	//append buf
	//
	return 0;
}

RespResponse *create_response(uint32_t hint_buf_size) {
	RespResponse* res = (RespResponse*)malloc(sizeof(RespResponse));	
	if (res == NULL)
		return NULL;
	memset(res, 0 , sizeof(RespResponse));
	
	res->buf_size = hint_buf_size;
	res->buf = (char *)malloc(hint_buf_size);
	if (res->buf == NULL) {
		free(res);
		return NULL;
	}

	return res;
}

void destroy_response(RespResponse *response) { 
	if (response == NULL)
		return;

	if (response->buf != NULL)
		free(response->buf);
	free(response);	
}


int encode_response_simplestring(RespResponse *response, int ok, const char *msg) {
	return 0;
}

