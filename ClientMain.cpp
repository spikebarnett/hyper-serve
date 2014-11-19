#include "ClientSocket.hpp"
#include "SocketException.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// http://tldp.org/LDP/LG/issue74/tougher.html

inline std::string cleanURL (std::string s)
{
  int pos = s.find ("//");
  if ( pos != std::string::npos )
  {
		return s.substr(pos + 2);
	}
	else
	{
		return s;
	}
}

std::string PULL (char* argv[])
{
  std::string host = "";
  std::string path = "";
  std::string URL = argv[2];
  URL = cleanURL(URL);
  int fpos = URL.find ('/');
  if ( fpos != std::string::npos )
  {
		host = URL.substr (0, fpos);
		path  = URL.substr (fpos);
	} else {
		host = URL;
		path  = "/";
	}

  try
	{
		ClientSocket client_socket ( host, 30000 );
		std::string reply;

		try
		{
			client_socket << "PULL " << path;
			client_socket >> reply;
			std::cout << reply << "\n";
		}
		catch ( SocketException& ) {}

		return reply;

	}
  catch ( SocketException& e )
	{
		std::cout << "Exception was caught:" << e.description() << "\n";
	}

  return "\0";

	//std::cout << "PULL\t" << host << "\t" << path << "\n";
}

void PUSH (char* argv[])
{
	std::cout << "PULL\t" << argv[0] << argv[1] << argv[2] << "\n";
}

int main ( int argc, char* argv[] )
{
	if ( argc > 2 )
	{
		std::string reqType = argv[1];
		if ( reqType == "PULL" )
		{
			PULL (argv);
		}
		else if ( reqType == "PUSH" )
		{
			PUSH (argv);
		}
		else
		{
			std::cout << "Usage: " << argv[0] << " <type> <content>\n";
		}
	}
	
	
	
	

	
	
	
	
	
	
	
	
	
	
  return 0;
  
  try
	{
		//72.52.245.244
		ClientSocket client_socket ( "localhost", 30000 );
		std::string reply;

		try
		{
			client_socket << "Client";
			client_socket >> reply;
		}
		catch ( SocketException& ) {}

		std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;

	}
  catch ( SocketException& e )
	{
		std::cout << "Exception was caught:" << e.description() << "\n";
	}

  return 0;
}
