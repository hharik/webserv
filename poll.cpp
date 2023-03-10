#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <iostream>

// listen for a new socket

int main()
{
	struct addrinfo hints, *ai;
	getaddrinfo(0, "1234", &hints, &ai);
	int socket_fd = socket(ai->ai_family, SOCK_STREAM, 0);
	if (bind(socket_fd, ai->ai_addr, ai->ai_addrlen) < 0)
		std::cout << " cant bind" << std::endl;
	if (listen(socket_fd, 127) < 0)
		std::cout << "can not listen" << std::endl;
	std::cout << "listening on 1234" << std::endl;
	struct pollfd pfd[1];
	pfd[0].events = POLLIN;
	pfd[0].fd = socket_fd;
	char buf[1024];
	int rv;
	while (1)
	{
		if ((rv = poll(pfd, 1, 100)) > 0)
		{
			if (pfd[0].revents & POLLIN)
			{

				send(socket_fd, buf, 200, 0);
				std::cout << "from Server "  << buf << std::endl;
				bzero(buf, sizeof(buf));
			}
			else if (pfd[0].revents & POLLOUT)
			{
				strcpy(buf, "HELOO WORLd");
				write(socket_fd, buf, sizeof(buf));
				break;
			}
		}
	}

}