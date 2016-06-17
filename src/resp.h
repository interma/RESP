/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/17/16 17:04:56 HKT
*/

#ifndef __interma_RESP_H
#define __interma_RESP_H

#include <stdint.h>

enum STATUS{
	ERR = -1,
	INIT = 0,
	OK,
	PART_OK,
	NOT_YET,
};

typedef struct{
	uint32_t off;
	uint32_t len;
}RespSlice;

typedef struct {
	int status;
	uint32_t argc;
	RespSlice *argv;

	char *buf;
	uint32_t buf_size;
	uint32_t pos;
}RespRequest;

RespRequest *create_request(uint32_t hint_buf_size);
void destroy_request(RespRequest *request);

#endif
