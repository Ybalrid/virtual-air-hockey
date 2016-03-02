#ifndef NETCFG
#define NETCFG

#include <string>

class NetConfig
{
	public:
	NetConfig()
	{
		IAmServer = true;
		useTCP = false;
		serverAddress = "127.0.0.1";
		port = 48161;
	}
	bool IAmServer;
	bool useTCP;//if false, use UDP
	std::string serverAddress;
	unsigned short port;
};

#endif //NETCFG