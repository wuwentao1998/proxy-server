# Proxy Server

This repository contains a concurrent proxy server, which can transmit GET request.

See the feature section for more feature information

## Envoirment
- OS: Mac OS

- Compiler: gcc 7.3.0 (-std=gnu99)

- Docker: 18.09.2

## Usage

``` bash
# with docker
./run.sh docker [port]

# without docker
./run.sh shell [port]
```

## Features
- Support concurrent access
- Start with one click
- Support formatted log
- Automatic unit test and mock test


## Unit Test
If you want to do unit test for your new feature, just add new .c unit test file in the dirctory 'test' and run the following command.

``` bash
./run_test.sh
```



## TODO
- Optimize the way and format of logging
- Support for more HTTP methods
- Add more unit test

## License

[MIT License](https://github.com/wuwentao1998/proxy-server/blob/master/LICENSE)
