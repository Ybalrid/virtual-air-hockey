#include <iostream>

#define	ASIO_STANDALONE
#include <asio.hpp>

using namespace std;

void pause()
{
	cout << "press enter to continue";
	cin.get();
}

int main(void)
{
	cout << "Hello networking world!"
		<< endl;

	cout << "Try to create an asio io_service : "
		<< endl;
	asio::io_service io_service;
	
	cout << "Okay, if this line is on the terminal, It means that it's working..."
		  << endl;

	pause();
	return EXIT_SUCCESS;
}