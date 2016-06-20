# RESP
A RESP(REdis Serialization Protocol) C Lib, it make tcp server parse resp request and send resp response easily.

##REdis Serialization Protocol

See: http://redis.io/topics/protocol, the key points:

>The way RESP is used in Redis as a request-response protocol is the following:

    Request: Clients send commands to a Redis server as a RESP Array of Bulk Strings.
    Response: The server replies with one of the RESP types according to the command implementation.

##example##


