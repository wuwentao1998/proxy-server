# Proxy Server

This repository contains a concurrent proxy server, which can transmit GET request.

See the feature section for more feature information

## Envoirment
- OS: Mac OS or Linux

- Compiler: gcc 7.3.0 (-std=gnu99)

- Docker: 18.09.2

## Usage

``` bash
# with Docker
./run.sh docker [port] [num of thread]

# without Docker
./run.sh shell [port] [num of thread]
```

PS: in Mac OS, you need to remove the comment below in file thread.h

``` bash
// #define MAC
``` 


## Features
- Support concurrent access using multithreading
- Reduce thread switching using thread pool
- Speed up access using cache
- Start with one click using shell script
- Support Docker startup 
- Support formatted log and automatically clean up
- Automatic unit test and mock test


## Unit Test
If you want to do unit test for your new feature, just add new .c unit test file in the dirctory 'test' and run the following command.

``` bash
./run_test.sh
```



## TODO
- Optimize the way and format of logging
- Support for more HTTP methods(POST, HEAD etc.)
- Optimize the thread pool
- Add pressure test

## License

[MIT License](https://github.com/wuwentao1998/proxy-server/blob/master/LICENSE)
