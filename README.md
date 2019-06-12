# Proxy Server

This repository contains a proxy server, which can transmit GET request. This proxy supports concurrent access using multiprocessing, multithreading and multiplexing. In addition, it takes advantage of cache to speed up access.

## Envoirment
- OS: Ubuntu 18.04.1

- Compiler: gcc 7.3.0

## Build
``` bash
cd ./src

make clean

make
```

## Usage
``` bash
./proxy [port]
```

## Unit Test
If you want to do unit test for your new feature, just add new .c unit test file in the dirctory 'test' and run the following command.

``` bash
./run_test.sh
```

## Features
- Automatic unit test


## License

[MIT License](https://github.com/tangbc/vue-virtual-scroll-list/blob/master/LICENSE)
