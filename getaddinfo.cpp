#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
	std::cout << "Enter Hostname to look up" << std::endl;	
	char hostname[54];
	std::cin >> hostname;
	int status;
	struct addrinfo *servinfo;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // ipv4
	hints.ai_socktype = SOCK_STREAM; //TCP 
	hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(hostname, "http", &hints, &servinfo)) != 0)
	{
		std::cout << "shit failed" << std::endl;
		exit(1);
	}
}