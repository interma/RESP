/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/17/16 17:04:56 HKT
*/

#ifndef __interma_RESP_H
#define __interma_RESP_H

#include <stdint.h>

enum STATE{
	ERR = -1,
	INIT = 0,
	NOT_YET,
	OK,
	PART_OK,
};

typedef struct{
	int off;
	int len;
}RespSlice;

typedef struct {
	int state;
	int argc;
	int cur_argc;
	RespSlice *argv;

	char *buf;
	uint32_t buf_size;
	uint32_t used_size;
	int pos;
	int skip_str;
}RespRequest;

typedef struct {
	char *buf;
	uint32_t buf_size;
	uint32_t len;
}RespResponse;

RespRequest *create_request(uint32_t hint_buf_size);
void destroy_request(RespRequest *request);
void reset_request(RespRequest *request);
int decode_request(RespRequest *request, const char *buf, uint32_t buf_len);
void print_request(RespRequest *request);

RespResponse *create_response(uint32_t hint_buf_size);
void destroy_response(RespResponse *response);
int encode_response_simplestring(RespResponse *response, int ok, const char *msg);

#endif

