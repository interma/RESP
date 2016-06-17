# RESP
A RESP(REdis Serialization Protocol) Lib, support c/py

##REdis Serialization Protocol

see:http://redis.io/topics/protocol

The way RESP is used in Redis as a request-response protocol is the following:

    Clients send commands to a Redis server as a RESP Array of Bulk Strings.
    The server replies with one of the RESP types according to the command implementation.

In RESP, the type of some data depends on the first byte:

    For Simple Strings the first byte of the reply is "+"
    For Errors the first byte of the reply is "-"
    For Integers the first byte of the reply is ":"
    For Bulk Strings the first byte of the reply is "$"
    For Arrays the first byte of the reply is "*"

