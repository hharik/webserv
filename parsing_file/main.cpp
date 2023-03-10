#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>


struct data {

};

class parsing
{
	private:
		int inside_main_bracket;
		int	location_flag;
		int count_locations;
		std::string filename;
		std::stack<std::string>	bracket;
		std::vector <std::vector<std::string> > matrix;
		std::vector <std::vector<std::string> > locations;
		std::vector<std::string> before_parse;
		std::map<std::string, std::vector<std::vector<std::string> > > data;
	public:

		parsing(std::string filename1) : inside_main_bracket(0), count_locations(0), data(), location_flag(0), filename(filename1), matrix(), bracket() { }

		std::string trim(std::string line, std::string whitespace)
		{
			int i = 0;
			std::size_t strbegin = line.find_first_not_of(whitespace);
			if (strbegin == std::string::npos)
				return " ";
			std::size_t strend = line.find_last_not_of(whitespace);
			std::size_t strRange = strend - strbegin + 1;
			return line.substr(strbegin, strRange);
		}

		std::string reduce_space(std::string line, std::string whitespace, std::string fill = " ")
		{
			line = trim(line, " \t\r\v\f\n");
			std::size_t beginspace =  line.find_first_of(whitespace);
			std::size_t endspace;
			while (beginspace != std::string::npos)
			{
				endspace = line.find_first_not_of(whitespace, beginspace);
				std::size_t range = endspace - beginspace;
				line.replace(beginspace, range, fill); // fill with only one space
				beginspace = line.find_first_of(whitespace, beginspace + fill.length());
			}
			size_t chel;
			if ((chel = line.find_first_of("{")) != std::string::npos)
			{
				if (line.find("{", chel + 1) != std::string::npos || line.find("}", chel + 1) != std::string::npos)
				{
					std::cout << "Error Brackets " << std::endl;
					exit(1);
				}
				bracket.push("{");
			}
			if ((chel = line.find("}")) != std::string::npos)
			{
				if (line.find("}", chel + 1) != std::string::npos || line.find("{", chel + 1)  != std::string::npos)
				{
					std::cout << "Error Brackets " << std::endl;
					exit(1);
				}
				if (bracket.size() == 0)
				{
					std::cout << "unclosed bracket" << std::endl;
					exit(1);
				}
				bracket.pop();
			}
			return line;
		}


		void split_and_assign(std::string line)
		{
			if (line.empty())
				return;
			size_t pos;
			std::stringstream ss(line);
			std::vector<std::string> tokens;
			std::string token;
			while (getline(ss, token, ' '))
				tokens.push_back(token);
			matrix.push_back(tokens);
		}
		
		template <class InputIterator> InputIterator advance_and_check(InputIterator first)
		{
			std::vector<std::string> tokens;
			while (((*first).find("}") == std::string::npos))
			{
				std::string token;
				std::stringstream ss(*first);
				while (getline(ss, token, ' '))
					tokens.push_back(token);
				first++;
			}
			tokens.push_back(*first);
			locations.push_back(tokens);
			return first;
		}

		void parse()
		{
			for (std::vector<std::string>::iterator it = before_parse.begin(); it != before_parse.end(); it++)
			{
				*it = reduce_space(*it, " \t\n\r\v\f", " ");
				split_and_assign(*it);
			}
			for (std::vector<std::string>::iterator it = before_parse.begin(); it != before_parse.end(); it++)
			{
				if ((*it).find("location") != std::string::npos)
					it = advance_and_check(it);
			}
			check();
		}

		void check() {
			if (bracket.size() > 0)
			{
				std::cout << "unclosed bracket" << std::endl;
			}
			// for (std::vector<std::vector<std::string> >::iterator it = matrix.begin(); it != matrix.end(); it++)
			// {
			// 	std::cout << (*it).at(0) << "*" << std::endl;
			// 	if((!(*it).at(0).find("server") && !(*it).at(0).find("server_name") && !(*it).at(0).find("root")  
			// 	&& !(*it).at(0).find("index") && !(*it).at(0).find("}") && !(*it).at(0).find("listen") && !(*it).at(0).find("max_body_size") && 
			// 	!(*it).at(0).find("auto_indexing") && !(*it).at(0).find("location") && !(*it).at(0).find("cgi") && !(*it).at(0).find("allowed_methods") 
			// 	&& !(*it).at(0).find("redirect") && !(*it).at(0).find("\n")) != std::string::npos)
			// 	{
			// 		std::cout << "ERR ";
			// 		exit(1);
			// 	}
			// }
		}

		void read_parse(){
			std::string buff;
			std::ifstream	ifs(filename);
			if (ifs.is_open())
			{
				while (getline(ifs, buff))
					before_parse.push_back(buff);
				parse();
				// std::cout << "MATRIX CONTENT " << matrix.size() << std::endl;
				// for(int i=0; i< matrix.size(); i++)
				// {
				// 	for(int j=0; j < matrix.at(i).size(); j++)
				// 	{
				// 		std::cout  << "*" << matrix[i][j] << "*" << std::endl;
				// 	}
				// }
				std::cout << "*** LOCATIONS MATRIX **  "  << locations.size() << std::endl;
				// for (int i = 0; i < locations.size(); i++)
				// {
				// 	for (int j = 0; j < locations[i].size(); j++)
				// 	{
				// 		std::cout << "*" << locations[i][j] << "*" << std::endl;
				// 	}
				// 	std::cout << std::endl;
				// }
			} else {
				std::cerr << "file Not Found" << std::endl;
				exit(1); 
			}
		}

	
};

int main()
{

	parsing a("config.conf");
	a.read_parse();

	// for (std::vector<std::string>::iterator it = a.before_parse.begin(); it != a.before_parse.end(); it++)
		// std::cout << *(a.before_parse.begin() + 1)  << std::endl;

}