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
```c
```

##other##

