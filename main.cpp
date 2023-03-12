#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include "parsing_file/parse.hpp"
#include <sys/ioctl.h>
#include <iostream>
#include <fcntl.h>
#include <vector>
#include "parsing_header.hpp"

int socket_create()
{
	int sc = socket(AF_INET, SOCK_STREAM, 0);
	int enable = 1;
	setsockopt(sc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
	struct sockaddr_in host_addr;
	host_addr.sin_family = AF_INET;
	host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	host_addr.sin_port = htons(9090);
	if (bind(sc,(struct sockaddr *)&host_addr, sizeof(host_addr)) != 0)
		std::cout << "bind failed" << std::endl;
	if (listen(sc, 10) != 0)
		std::cout << "Listen failed" <<std::endl;
	return sc;
}

void set_nonblocking(int socket_fd)
{
	int flag = fcntl(socket_fd, F_GETFL, 0);
	fcntl(socket_fd, F_SETFL, flag | O_NONBLOCK);
}



	// std::cout << token << std::endl;
	// 	std::string line;
	// 	std::stringstream ss1(token);
	// 	std::vector<std::string> tokens;
	// 	while (getline(ss1, line, ' '))
	// 	{		
	// 		std::cout << line << std::endl;
	// 		if (line.length() == 1)
	// 		{
	// 			// parse_body
	// 		}
	// 		tokens.push_back(line);
	// 	}
	// 	matrix.push_back(tokens);
	// 	// check_header(token);
	
	// }
	// // if (matrix[0][0].find_first_not_of("GET DELETE POST") != std::string::npos)
	// {
	// 	std::string not_response = "HTTP/1.1 501 Bad Request\r\nContent-Type: text/html\r\n\r\n";
	// 	not_response += "<html><body><h1>501 - Not Implemented!</h1></body></html>";
	// 	send(client_fd,not_response.c_str(), not_response.size(), 0);
	// 	return;
	// }
	// if (matrix.at(0).size() > 3 || matrix[0][1].find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=") != std::string::npos) 
	// {
	// 	std::string bad_response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n";
	// 	bad_response += "<html><body><h1>400 - Bad Request!</h1></body></html>";
	// 	send(client_fd, bad_response.c_str(), bad_response.size(), 0);
	// 	return;
	// }
	// if (matrix[0][1].size() > 2048)
	// {
	// 	std::string to_long = "HTTP/1.1 414 Request-URI Too Long \r\nContent-Type: text/html\r\n\r\n";
	// 	to_long += "<html><body><h1>414 Request URI Too long!</h1></body></html>";
	// 	send(client_fd, to_long.c_str(), to_long.size(), 0);
	// }









int main()
{
	parsing_header r;
	parsing conf;
	conf.read_parse();
	int socket_fd = socket_create();
	std::cout << "listening on port 9090" << std::endl;
	// set_nonblocking(socket_fd);
	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                    response += "<html><body><h1>Hello, World!</h1></body></html>";
	for (;;)
	{
		char buffer[8000];
		std::string req;
		struct sockaddr_in host_addr;
		int new_socket = accept(socket_fd, (struct sockaddr *)&host_addr, (socklen_t *)&host_addr);
		if (new_socket < 0)
		{
			std::cout << "failed to accept socket " << socket_fd << std::endl;
			continue;
		}
		std::cout << "connection established" << std::endl;
		int valread = recv(new_socket, buffer, sizeof(buffer), 0);
		if (valread < 0)
		{
			perror("webserver (accept)");
			continue;
		}
		req = buffer;
		r.parse_header(req, new_socket);
		// send(new_socket, response.c_str(), response.size(), 0);
		close(new_socket);
	}
}
