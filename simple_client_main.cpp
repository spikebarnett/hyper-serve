#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

// http://tldp.org/LDP/LG/issue74/tougher.html


int main ( int argc, char* argv[] )
{

	//url = argv[0];
  
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
