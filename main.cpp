#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

// 1: create a socket
// 2 : “assigning a name to a socket” (bind)
// 3 : listen for incoming connections (listen)

//  struct sockaddr_in {
//               sa_family_t    sin_family; /* address family: AF_INET */
//               in_port_t      sin_port;   /* port in network byte order */
// 		 struct in_addr sin_addr;   /* internet address */
//           };
//           /* Internet address */
//           struct in_addr {
//               uint32_t       s_addr;     /* address in network byte order */
//           }; 
int main()
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	const int enable = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		std::cout << "setsockopt failed " << std::endl;
	struct sockaddr_in host_addr;
	host_addr.sin_family = AF_INET;
	host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	host_addr.sin_port = htons(9090);// htons(9099)
	if (bind(socket_fd, (struct sockaddr *)&host_addr, sizeof(host_addr)) != 0)
	{
		std::cout << "error in binding socket" << std::endl;
		return -1;
	}
	if (listen(socket_fd, 128) != 0)
		std::cout << "error in listening socket" << std::endl;
	unsigned int size = sizeof(host_addr);
	char res[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
				  "Content-type: text/html\r\n\r\n"
                  "<html>hello, world</html>\r\n";
	struct sockaddr_in client_addr;
	std::cout << "Listening on port 9090" << std::endl;
	while (1)
	{
		int new_socket = accept(socket_fd,(struct sockaddr *)&host_addr, &size);
		std::cout << " ---- " << new_socket << std::endl;
		// int sockn;
		// if ((sockn = getsockname(new_socket,(struct sockaddr *)&client_addr, (socklen_t *)&client_addr)) < 0)
		// 	std::cout << "failed to get socket name" << std::endl;
		if (new_socket > 0)
		{
			char buf[1000];
			bzero(buf,sizeof(buf));
			read(new_socket, buf, sizeof(buf));
			std::cout << buf << std::endl;
			int walt = write(new_socket, res, strlen(res));
			if (walt < 0)
				std::cout << "shittt not sent" << std::endl;
		}
		for (long long i = 0; i < LLONG_MAX; i++)
			write(new_socket, "a", 1);
		for (long long i = 0; i < LLONG_MAX; i++)
			write(new_socket, "a", 1);
		close(new_socket);
	}
	// if (socket_fd < 0)
	// 	std::cout << "socket failed " << std::endl;
	// else
	// 	std::cout << "socket created " << socket_fd << std::endl;
}
