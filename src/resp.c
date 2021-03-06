/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/17/16 17:04:52 HKT
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "resp.h"

//==Request==

RespRequest *create_request(uint32_t hint_buf_size) {
	RespRequest* req = (RespRequest*)malloc(sizeof(RespRequest));	
	if (req == NULL)
		return NULL;
	memset(req, 0 , sizeof(RespRequest));
	
	req->state = INIT;
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

static int get_segment(RespRequest* request) {
	RespRequest *r = request;
	char *p = r->buf+r->pos; 
	//while (p-r->buf <= r->buf_size && !(*p == '\r' && *(p+1) == '\n')) {
	while (p != r->buf+r->buf_size && !(*p == '\r' && *(p+1) == '\n')) {
		p++;
	}

	if (p == r->buf+r->buf_size)
		return -1;
	int seg_end = p - r->buf;
	r->buf[seg_end] = '\0'; //cut as c NULL-termialed string
	return seg_end;
}

//example:*3\r\n$3\r\nfoo\r\n$-1\r\n$3\r\nbar\r\n
int decode_request(RespRequest *request, const char *buf, uint32_t buf_len) {
	RespRequest *r = request;

	//append buf
	if (buf_len+r->used_size > r->buf_size) {
		//malloc new
		uint32_t new_size = r->buf_size;
		while (new_size < r->used_size+buf_len)
			new_size *= 2; //double 
		char *more_buf = (char *)realloc(r->buf, new_size);
		if (more_buf == NULL)
			return -1;
		r->buf = more_buf;
		r->buf_size = new_size;
	}
	//just append
	memcpy(r->buf+r->used_size, buf, buf_len);		
	r->used_size += buf_len;
	
	int seg_end = -1;
	while (seg_end = get_segment(r), seg_end > 0) {
		//[pos, seg_end) is a segment

		//*num
		if (r->argc == 0 && r->state == INIT) {
			//init argc,argv
			if (r->buf[r->pos] != '*') {
				r->state = ERR;
				return 0;
			}
			
			r->argc = atoi(r->buf+r->pos+1);
			if (r->argc < 0) {
				r->state = ERR;
				return 0;
			}
			r->argv = (RespSlice *)calloc(sizeof(RespSlice), r->argc); //set buf 0
			if (r->argv == 0)
				return -1;
			
			r->state = NOT_YET;
		}
		//$num str
		else {
			//$num
			if (!r->skip_str && r->buf[r->pos] == '$') {
				int len = atoi(r->buf+r->pos+1);
				r->argv[r->cur_argc].off = seg_end+2;
				r->argv[r->cur_argc].len = len;
				r->skip_str = 1;
			}
			//str
			else {
				r->skip_str = 0;
				r->cur_argc++;
			}
		}
		
		r->pos = seg_end+2;
		if (r->argc > 0 && r->argc <= r->cur_argc)
			break;
	}

	//check request	

	//judge remain
	if (r->cur_argc < r->argc) {
		//NOT_YET
	}
	else {
		if (r->pos+2 >= r->used_size)
			r->state = OK;
		else
			r->state = PART_OK;
	}
	return 0;
}

void print_request(RespRequest *request) {
	int show_argv = 1;
	RespRequest *r = request;
	printf("state[%d] argc[%d] cur_argc[%d] ", r->state,r->argc,r->cur_argc);
	printf("buf_size[%u] used_size[%u] pos[%u]", r->buf_size,r->used_size,r->pos);

	for (int i = 0; i < r->argc; i++) 
		printf(" argv%d[%d][%d]", i, r->argv[i].off, r->argv[i].len);

	printf("\n");
	if (show_argv) {
		printf("REQUEST:");
		for (int i = 0; i < r->argc; i++)
			printf("%s ", r->buf+r->argv[i].off);

		printf("\n");
	}
}

void reset_request(RespRequest *request) {
	if (request == NULL)
		return;

	RespRequest *r = request;
	r->state = INIT;
	r->argc = r->cur_argc = 0;
	if (r->argv != NULL)
		free(r->argv);
	r->argv = NULL;
	
	r->used_size = 0;
	r->pos = 0;
	r->skip_str = 0;

	//reuse buf&buf_size
}

//==Response==

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

void reset_response(RespResponse *response) {
	if (response == NULL)
		return;

	response->used_size = 0;
	//reuse buf&buf_size
}

static int try_enlarge(RespResponse *r, uint32_t add_size) {
	//malloc new
	uint32_t new_size = r->buf_size;
	while (new_size < r->used_size+add_size)
		new_size *= 2; //double 
	char *more_buf = (char *)realloc(r->buf, new_size);
	if (more_buf == NULL)
		return -1;
	r->buf = more_buf;
	r->buf_size = new_size;
	return 0;
}
static void buf_append(RespResponse *r, const char *buf, uint32_t buf_size) {
	memcpy(r->buf+r->used_size, buf, buf_size);		
	r->used_size += buf_size;
}

//-WRONGTYPE Operation against a key holding the wrong kind of value\r\n
int encode_response_status(RespResponse *response, int ok, const char *msg) {
	RespResponse *r = response;	
	
	uint32_t len = strlen(msg)+3; //+msg\r\n
	if (try_enlarge(r, len) < 0) {
		return -1;
	}	
	if (ok) 
		buf_append(r, "+", 1);
	else
		buf_append(r, "-", 1);
	buf_append(r, msg, len-3);
	buf_append(r, SEP, strlen(SEP));
	return 0;
}

//:-1000\r\n
int encode_response_integer(RespResponse *response, int num) {
	RespResponse *r = response;	
	char buf[16];
	sprintf(buf, ":%d"SEP, num);
	uint32_t len = strlen(buf);
	if (try_enlarge(r, len) < 0) {
		return -1;
	}	
	buf_append(r, buf, len);
	return 0;
}
//$6\r\nfoobar\r\n
int encode_response_string(RespResponse *response, const char *str, uint32_t size) {
	RespResponse *r = response;	
	char buf[16];
	sprintf(buf, "$%u"SEP, size);
	uint32_t len = strlen(buf)+size+strlen(SEP);
	if (try_enlarge(r, len) < 0) {
		return -1;
	}	
	buf_append(r, buf, strlen(buf));
	buf_append(r, str, size);
	buf_append(r, SEP, strlen(SEP));
	return 0;
}

//*0\r\n
//*2\r\n$3\r\nfoo\r\n$3\r\nbar\r\n
int encode_response_array(RespResponse *response, int num) {
	RespResponse *r = response;	
	char buf[16];
	sprintf(buf, "*%d"SEP, num);
	uint32_t len = strlen(buf);
	if (try_enlarge(r, len) < 0) {
		return -1;
	}	
	buf_append(r, buf, len);
	return 0;
}

