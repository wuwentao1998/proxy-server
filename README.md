# Proxy Server

This repository contains a proxy server, which can transmit GET request. This proxy supports concurrent access using multiprocessing, multithreading and multiplexing. In addition, it takes advantage of cache to speed up access.

## Envoirment
- OS: Ubuntu 18.04.1

- Compiler: gcc 7.3.0

- Docker: 18.09.2

## Usage
- with docker
``` bash
./run.sh [port]
```
- without docker
``` bash
cd ./src
make
make clean
./proxy [port]&
```

## Unit Test
If you want to do unit test for your new feature, just add new .c unit test file in the dirctory 'test' and run the following command.

``` bash
./run_test.sh
```

## Features
- Start with one click using docker
- Support formatted log
- Automatic unit test


## License

[MIT License](https://github.com/tangbc/vue-virtual-scroll-list/blob/master/LICENSE)
