#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>



int main()
{
	int key = kqueue();
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	const int enable = 1;

	//set reuse address for socket
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		std::cout << "Socket error: " << std::endl;
	struct  sockaddr_in host_addr;
	host_addr.sin_family = AF_INET;
	host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	host_addr.sin_port = htons(9090);
	if (bind(socket_fd, (struct sockaddr *)&host_addr, sizeof(host_addr)) != 0)
		std::cout << "Bind failed : " << std::endl;
	if (listen(socket_fd, 0) != 0)
		std::cout << "Listen failed : " << std::endl;
	std::cout << "listening on 9090" << std::endl;
	struct kevent events[1];
	EV_SET(&events[0], socket_fd, EVFILT_READ, EV_ADD, 0, 0, 0);
	// if (ret == -1)
	// 	std::cout << "kevent failed : " << std::endl;
	std::cout << " \neee" << std::endl;
	char buff[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
				  "Content-type: text/html\r\n\r\n"
                  "<html>hello, world</html>\r\n";
	while (1)
	{
		int ret = kevent(key, events, 0, NULL, 0, 0);
		if( ret == -1)
		{
			std::cout << "something went wrong" << std::endl;
			exit(1);
		}
		for (int i = 0; i < ret; i++)
		{ 
			if (events[i].filter == EVFILT_READ)
			{
				int new_socket = accept(events[i].ident, NULL,0);
				char buffer[1024];
				memset(buffer, 0, 1024);
				read(new_socket, buffer ,sizeof(buffer));
				close(new_socket);
			}
		}
	}
}