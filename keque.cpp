#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/ioctl.h>
#include <iostream>
#include <fcntl.h>
#include <vector>


int create_socket()
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	int enable = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9090);
	if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr) ) <  0)
		std::cout << "bind failed" << std::endl;
	if (listen(socket_fd, 10) < 0)
		std::cout << "listen failed" << std::endl;
	return socket_fd;
}

void set_nonblocking(int socket_fd)
{
	int flags = fcntl(socket_fd, F_GETFL, 0);
	fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
}

int main()
{
	int socket_fd =	 create_socket();
	set_nonblocking(socket_fd);

	// keque
	int kq = kqueue();
	if (kq < 0)
		std::cout << "Error: kqueue " << std::endl;

	struct kevent ev;
	EV_SET(&ev, socket_fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0,0, NULL);
	if (kevent(kq, &ev, 1, NULL, 0, 0) < 0)
		std::cout << "First kevent failed" << std::endl;
	std::vector<int>  client_fds;
	while (true)
	{
		struct kevent events[1024];
		int nev;
		if ((nev = kevent(kq, NULL, 0, events, 1024, NULL)) < 0) 
		{
			std::cout << "second kevent failed" << std::endl;
			exit(1);
		}
		for (int i = 0; i < nev ; i++)
		{
			if (events[i].ident == socket_fd && events[i].filter == EVFILT_READ)
			{
				struct sockaddr_in client_addr;
				socklen_t client_len = sizeof(client_addr);
				int client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_len);
				std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                    response += "<html><body><h1>Hello, World!</h1></body></html>";
				if (client_fd < 0)
					std::cout << "Failed to accept" << std::endl;
				send(client_fd, );
				// else {
				// 	set_nonblocking(client_fd);
				// 	EV_SET(&ev, client_fd, EVFILT_READ, EV_ADD, 0,0, NULL);
				// 	if (kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
				// 		std::cout << "ERROR RRRR " << std::endl;
				// 	else
				// 		client_fds.push_back(client_fd);
				// }
			}
			else if (events[i].ident != socket_fd &&  events[i].filter == EVFILT_READ)
			{
				char buf[1024];
				int nbytes = recv(events[i].ident, buf, 1024, 0);
				std::cout << buf << std::endl;
				if (nbytes < 0)
					std::cout << "ERRORR" << std::endl;
				else if (nbytes == 0)
				{
					std::cout << "finished" << std::endl;
					close(events[i].ident);
				}
				else {
					
					send(events[i].ident, response.c_str(), response.size(),0);
					// for (long long i = 0; i < LONG_MAX; i++)
						// write(events[i].ident, "a", 1);
					close(events[i].ident);
				}
			}
		close(events[i].ident);
		}
	}
}
