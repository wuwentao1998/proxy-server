#include <assert.h>
#include <fcntl.h>
#include "../src/robust_IO.h"

void test_1()
{
    int fd = open("./input/robust_IO_test.in", O_RDONLY);
    char buffer[1024];
    ssize_t count = Rio_readn(fd, buffer, 10);
    assert(count == 10);
    assert(strcmp(buffer, "123456 789") == 0);
    close(fd);
}

int main(void)
{
    test_1();
}