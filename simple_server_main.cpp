#include "ServerSocket.hpp"
#include "SocketException.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>

// http://tldp.org/LDP/LG/issue74/tougher.html

std::string DocRoot = "/home/cbarnett/git/hyper-serve";


inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
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
						int fpos = req.find (" ", 5);
						std::string path = req.substr (5, fpos - 5);
						path = DocRoot + path;
						
						if (file_exists(path))
						{
							std::ifstream file(path.c_str());
							if (file.is_open())
							{
							  std::stringstream buffer;
								buffer << file.rdbuf();
								new_sock << buffer.str();
							}
							
							std::cout << path << ": Served.\n";
						}
						else
						{
							std::cout << path << ": No such file or file unreadable.\n";
							new_sock << path << ": No such file or file unreadable.";
						}
						
						
						
						
						new_sock << "";
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
