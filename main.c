#include "connect.h"


int main(int argc, char** argv)
{
	if (argc < 2)
    {
		fprintf(stderr, "usage: %s <port number to bind and listen>\n", argv[0]);
		exit(1);
	}

	int listenfd = Open_listenfd(argv[1]);
	while (True)
	{

	}

	return 0;
}
