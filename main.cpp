#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
class parsing
{
	private:
		int inside_main_bracket;
		std::string filename;
		std::stack<std::string>	bracket;
		std::vector <std::vector<std::string> > matrix;
		std::vector<std::string> before_parse;
		std::map<std::string, std::vector<std::string> *> data;
	public:

		parsing(std::string filename1) : inside_main_bracket(0),  data(), filename(filename1), matrix(), bracket() { }
		
		std::string trim(std::string line, std::string whitespace)
		{
			int i = 0;
			if (line.length() == 0)
				return " ";
			std::size_t strbegin = line.find_first_not_of(whitespace);
			std::size_t strend = line.find_last_not_of(whitespace);
			std::size_t strRange = strend - strbegin + 1;
			return line.substr(strbegin, strRange);
		}

		std::string reduce_space(std::string line, std::string whitespace, std::string fill = " ")
		{
			line = trim(line, " \t\r\v\f");
			std::size_t beginspace =  line.find_first_of(whitespace);
			std::size_t endspace;
			while (beginspace != std::string::npos)
			{
				endspace = line.find_first_not_of(whitespace, beginspace);
				std::size_t range = endspace - beginspace;
				line.replace(beginspace, range, fill); // fill with only one space

				beginspace = line.find_first_of(whitespace, beginspace + fill.length());
			}
			// if (line.find_first_of("{") != std::string::npos)
			// 	bracket.push("{");
			// else if (line.find_first_of("}") != std::string::npos)
			// 	bracket.pop();
			return line;
		}

		void split_and_assign(std::string line, size_t i)
		{
			std::size_t key_;
			std::cout << line << std::endl;
			if ((key_ = line.find(" ")) != std::string::npos)
				std::cout << line.substr(key_);
		}

		void parse()
		{
			int i = 0;
			for (std::vector<std::string>::iterator it = before_parse.begin(); it != before_parse.end(); it++)
			{
				// std::cout << *it << std::endl;
				*it = reduce_space(*it, " \t\n\r\v\f\n", " ");
				std::cout << *it << std::endl;
 				// split_and_assign(*it, i);
				i++;
			}
		}
		void check() {
			if (bracket.size() > 0)
			{
				std::cout << "unclosed bracket" << std::endl;
			}
		}
		void read_parse(){
			std::string buff;
			std::ifstream	ifs(filename);
			if (ifs.is_open())
			{
				while (getline(ifs, buff))
					before_parse.push_back(buff);
				parse();
				check();
				for (std::vector<std::vector<std::string> >::iterator it = matrix.begin(); it != matrix.end(); it++)
					std::cout << *(it)->begin() << std::endl;
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