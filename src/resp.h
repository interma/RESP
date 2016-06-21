/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/17/16 17:04:56 HKT
*/

#ifndef __interma_RESP_H
#define __interma_RESP_H

#include <stdint.h>

#define SEP "\r\n" 
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
	uint32_t used_size;
}RespResponse;

RespRequest *create_request(uint32_t hint_buf_size);
void destroy_request(RespRequest *request);
void reset_request(RespRequest *request);
int decode_request(RespRequest *request, const char *buf, uint32_t buf_len);
void print_request(RespRequest *request);

RespResponse *create_response(uint32_t hint_buf_size);
void destroy_response(RespResponse *response);
void reset_response(RespResponse *response);

/*
For Simple Strings the first byte of the reply is "+"
For Errors the first byte of the reply is "-"
For Integers the first byte of the reply is ":"
For Bulk Strings the first byte of the reply is "$"
For Arrays the first byte of the reply is "*"
*/
int encode_response_status(RespResponse *response, int ok, const char *msg);
int encode_response_integer(RespResponse *response, int num);
int encode_response_string(RespResponse *response, const char *str, uint32_t size);
int encode_response_array(RespResponse *response, int num);

#endif

