// ENet
#include <enet/enet.h>

// Protocol Buffer Files
#include "config.h"
// System Headers
#include <iostream>

int main(int argc, char** argv)
{

  if(enet_initialize() != 0)
  {
    std::cerr << "Failed to initialize ENet." << std::endl;
    return EXIT_FAILURE;
  }

  atexit(enet_deinitialize);

  if(argc < 2)
  {
    std::cerr << "Usage: server:port" << std::endl;
    return EXIT_FAILURE;
  }

  size_t argv1Len = strlen(argv[1]);
  size_t colonLoc = strcspn(argv[1], ":");
  size_t portLen = argv1Len - colonLoc - 1;

  char host[BUFFERSIZE];
  char c_port[BUFFERSIZE];

  strncpy(host, argv[1], colonLoc);
  host[colonLoc] = '\0';
  strncpy(c_port, argv[1] + colonLoc + 1, portLen);
  c_port[portLen] = '\0';
  int port = atoi(c_port);

  if(port == 0)
  {
    std::cerr << "Invalid port: " << c_port << std::endl;
    return EXIT_FAILURE;
  }

  ENetHost* client = enet_host_create(NULL, 1, 2, 0, 0);

  if(client == NULL)
  {
    std::cerr << "Failed to create ENet client." << std::endl;
    return EXIT_FAILURE;
  }

  ENetAddress address;
  enet_address_set_host(&address, host);
  address.port = port;

  std::cout << "Attempting to connect to " << host << ":" << port << " ... ";
  std::cout.flush();

  ENetPeer* server = enet_host_connect(client, &address, 2, 0);

  if(server == NULL)
  {
    std::cerr << "failed" << std::endl;
    return EXIT_FAILURE;
  }

  ENetEvent event;

  // wait 5000 ms for a connection
  if(enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
  {
    std::cout << "done" << std::endl;
  }
  else
  {
    enet_peer_reset(server);
    std::cout << "failed" << std::endl;
    return EXIT_FAILURE;
  }

  while(true)
  {
    if(enet_host_service(client, &event, 1000) > 0)
    {
      switch(event.type)
      {
        case ENET_EVENT_TYPE_DISCONNECT:
        {
          std::cout << "disconnected" << std::endl;
          exit(EXIT_SUCCESS);
          break;
        }

        case ENET_EVENT_TYPE_RECEIVE:
        {

        }
      }
    }
  }

  return 0;
}