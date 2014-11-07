#include "ServerSocket.h"
#include "SocketException.h"
#include <iostream>
#include <fstream>
#include <string>

// http://tldp.org/LDP/LG/issue74/tougher.html

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
					std::string data;
					new_sock >> data;
					if ( data == "Client" ) new_sock << "Server";
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
