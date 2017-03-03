# CS 130 TEAM TBD Web Server
### Contributors
* Alex Waz
* Derek Nguyen
* Sung Hyun Yoon

## How to build, test, and run

### To build the server
```
make
```

### To build and run tests
```
make test
```

### To remove object and binary files
```
make clean
```

### To analyze test coverage
```
make coverage
```

### To run the integration test
```
make integration
```

### To run the server after building it
```
./server <config_file>
```


## Source Code Layout
* All the source code is in the base directory.
* The main function is declared in `server_main.cc`.
* `Request` and `Response` classes are defined in `request_handler.h` and `request_handler.cc`.
* Other than these two, each `.h` and `.cc` file only contains definition of a single class.
* Unit tests for `ExampleClass` are in `example_class_test.cc`.
* `path_config` is an example config file used for assignment 6.
* `server_integration_test.py` is our integration test script.
* The following files are only used for testing: `example`, `example_config`, `test.txt`, and files under `hw4/`.

## ProxyHandler by Team05
Team05 has added a ProxyHandler. It takes in its configuration a URL. Upon  
request, the handler gets the body of the response from the given URL and in  
turn sends it back to the client. It will handle redirection response codes  
by recursively attempting to do this until it receives a valid response. Links  
clicked on pages displayed when using the proxy will continue to use the  
proxy. If testing the server with "./server path_config", the UCLA website can  
be displayed by going to localhost:8080/reverse_proxy in a browser.
