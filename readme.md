## me-mqtt

## Ideas

Perhaps a shared_ptr<std::string const> to minimize string copying...

Wrap all in namespaces 'me'.

## Compiling

This uses standalone boost::asio. `ASIO_STANDALONE` must be defined in the compiler definitions in order to compile unless boost is installed.