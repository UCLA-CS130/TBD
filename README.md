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
* The main function is decalared in `server_main.cc`.
* `Request` and `Response` classes are defined in `request_handler.h` and `request_handler.cc`.
* Other than these two, each `.h` and `.cc` file only contains definition of a single class.
* Unit tests for `ExampleClass` are in `example_class_test.cc`.
* `path_config` is an example config file used for assignment 6.
* `server_integration_test.py` is our integration test script.
* The following files are only used for testing: `example`, `example_config`, `test.txt`, and files under `hw4/`.
