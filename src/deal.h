#ifndef DEAL_H
#define DEAL_H
#include <stdbool.h>
#include "error.h"

void deal(int connectfd);
void ignore_header(rio_t* rio);

#endif