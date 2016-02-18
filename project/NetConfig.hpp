#ifndef NETCFG
#define NETCFG

#include <string>

class NetConfig
{
	public:
	NetConfig()
	{
	}
	bool IAmServer;
	bool useTCP;//if false, use UDP
	std::string serverAddress;
};

#endif //NETCFG