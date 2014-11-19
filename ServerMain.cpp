#include "ServerSocket.hpp"
#include "SocketException.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <ctype.h>

// http://tldp.org/LDP/LG/issue74/tougher.html

// g++ -std=c++11 -Wall -o "%e" "%f"


std::string DocRoot = "/home/cbarnett/git/hyper-serve";


template <typename T>
T StringToNumber ( const std::string &Text, T defValue = T() )
{
    std::stringstream ss;
    for ( std::string::const_iterator i=Text.begin(); i!=Text.end(); ++i )
        if ( isdigit(*i) || *i=='e' || *i=='-' || *i=='+' || *i=='.' )
            ss << *i;
    T result;
    return ss >> result ? result : defValue;
}

inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

std::ifstream::pos_type file_size(const std::string filename)
{
    std::ifstream in(filename.c_str(), std::ios::binary|std::ios::ate);
    return in.tellg(); 
}

std::string PULL(std::string req)
{
	int fpos = req.find (" ");				// File position
	int apos = req.find ("#", fpos);	// Ancor position
	int rpos = req.find ("@", fpos);	// Range position
	std::string path, ancor, range;
	if ((apos==std::string::npos)&&(rpos==std::string::npos))
	{
		path  = req.substr (fpos + 1);
		ancor = "";
		range = "";
	}
	else if (rpos==std::string::npos)
	{
		path  = req.substr (fpos + 1, apos - fpos - 1);
		ancor = req.substr (apos);
	}
	else if (apos==std::string::npos)
	{
		path  = req.substr (fpos + 1, rpos - fpos - 1);
		range = req.substr (rpos);
	}
	std::string fullpath = DocRoot + path;
	if (file_exists(fullpath))
	{
		if ((ancor=="")&&(range==""))
		{
			std::ifstream file(fullpath.c_str(), std::ios::in);
			if (file.is_open())
			{
				std::stringstream buffer; std::string contents;
				buffer << file.rdbuf();
				contents = buffer.str();
				file.close();
				std::cout << fullpath << ": Served.\n";
				return contents;
			}
			else
			{
				std::cout << fullpath << ": Could not be opened.\n";
				return "Error opening" + path + ".";
			}
		}
		else if (ancor!="")
		{
			std::ifstream file(fullpath.c_str(), std::ios::in);
			if (file.is_open())
			{
				std::string line, contents = "";
				bool ancor_found = false, bracket_found = false;
				while ( getline (file,line) )
				{
					if (ancor_found)
					{
						if (bracket_found)
						{
							if (line=="}}")
							{
								file.close();
								std::cout << fullpath << ancor << ": Served.\n";
								return contents;
							}	else {
								contents += line + "\n";
							}
						} else {
							if (line=="{{") bracket_found = true;
						}
					} else {
						if (line==ancor) ancor_found = true;
					}
				}
				std::cout << fullpath << ancor << ": Ancor not found.\n";
				return ancor + " not found in " + path + ".";
			} else {
				std::cout << fullpath << ": Could not be opened.\n";
				return "Error opening" + path + ".";
			}
		}
		else if (range!="")
		{
			int cpos = range.find (",");	// Comma position
			std::string beg = range.substr (1, cpos - 1);
			std::string end = range.substr (cpos + 1);
			
			std::string contents = "";
			std::ifstream file(fullpath.c_str(), std::ios::in|std::ios::binary);
			int size = file_size(fullpath);
			
			if (file.is_open())
			{
				int i_beg,i_end;
				
				if (beg=="^") i_beg = 0; else i_beg = StringToNumber(beg,0);
				if (end=="$") i_end=size; else i_end = StringToNumber(end,0);
				if (i_beg>i_end) i_end=i_beg;
				
				char ch; int i = 0;
				while (file >> std::noskipws >> ch)
				{
					if ((i_beg <= i)&&(i <= i_end)) contents += ch;
					if (i>=i_end) break; ++i;
				}
				file.close();
				std::cout << fullpath << range << ": Served.\n";
				return contents;
			} else {
				std::cout << fullpath << ": Could not be opened.\n";
				return "Error opening" + path + ".";
			}	
			
		}
	}
	else
	{
		std::cout << path << ": No such file or file unreadable.\n";
		return "Error: No such file or file unreadable.";
	}
}


int main ( int argc, char* argv[] )
{
  std::cout << "running....\n";

  try
	{
		// Create the socket
		ServerSocket server ( 30000 );

		while ( true )
		{

			ServerSocket new_sock;
			server.accept ( new_sock );

			try
			{
				while ( true )
				{
					std::string req;
					new_sock >> req;

					if ( req.substr(0, 4) == "PULL" )
					{
						new_sock << PULL(req);
					}
					else new_sock << "fail";
				}
			}
			catch ( SocketException& ) {}

		}
	}
  catch ( SocketException& e )
	{
		std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
	}

  return 0;
}
