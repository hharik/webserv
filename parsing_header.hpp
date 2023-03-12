#ifndef PARSING_HEADER_HPP_
#define PARSING_HEADER_HPP_
#include <iostream>
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
#include <utility>
#include <fcntl.h>
#include <map>
#include <vector>

struct REQ_data{ 
	std::string method;
	std::string uri;
	std::string http_version;
};





class parsing_header {
	public :
	REQ_data req;
	std::map<std::string, std::string> data_headers;
	int client;
	parsing_header(): req() {}
	void check_get()
	{
		if (req.method == "GET")
		{
			// if (req.uri.find_first_not_of("-_.~!*'();:@&=+$,/?%#[]?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos)
			// {

			// 	send(client, );
			// }
		}
	}
	void parse_header(std::string headers, int client_fd) {
		std::string token;
		client = client_fd;
		std::stringstream ss(headers);
			// std::cout << headers << std::endl;
		getline(ss, token, '\n');
		std::stringstream qq(token);
		int i = 0;
		struct	REQ_data req;
		while (getline(qq, token, ' '))
		{
			if (i == 0)
				req.method = token;
			else if (i == 1)
				req.uri = token;
			else if (i == 2)
				req.http_version = token;
			i++;
		}
		std::string bad_response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n";
		bad_response += "<html><body><h1>400 - Bad Request!</h1></body></html>";
		std::string uri_too_long = " HTTP/1.0 414 Request-URI Too long\r\nContent-Type: text/html\r\n\r\n";
		uri_too_long += "<html><body><h1> 414 Request-URI Too long</h1></body></html>";
		if (i > 3 || (req.method == "GET" && req.uri.find_first_not_of("-_.~!*'();:@&=+$,/?%#[]?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos))
			send(client_fd, bad_response.c_str(), bad_response.size(), 0);
		if (req.uri.length() > 2048)
			send(client_fd, uri_too_long.c_str(), uri_too_long.size(), 0);
		std::vector<std::string> tokens;
		while (getline(ss, token, '\n'))
			tokens.push_back(token);
		for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end();it++)
		{
			size_t indx;
			if ((*it).length() == 1) // to stop parsing the headers and move to the body
				break ;
			if ((indx = (*it).find(":")) != std::string::npos)
			{
				data_headers.insert(std::make_pair((*it).substr(0, indx), (*it).substr(indx + 1, (*it).length())));
			}
		}
		for (std::map<std::string, std::string>::iterator it = data_headers.begin(); it != data_headers.end(); it++)
			std::cout << it->first << " " << it->second << std::endl;
		if (req.method == "POST")
		{
			std::cout << "rrrr "<< std::endl;
			//error parse body of data 
		}
		check_get();
		return ;
	}

	void exec_path()
	{
		
	}
};


#endif