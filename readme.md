## me-mqtt

## Next Steps

Most MQTT 3.1.1 features supported. Needs better error handling. lots of refactoring.

Need to implement keepalive timeout.

Broadcaster should be multithreaded, only lock on sub/unsub...

## Compiling

This uses standalone boost::asio. `ASIO_STANDALONE` must be defined in the compiler definitions in order to compile unless boost is installed.