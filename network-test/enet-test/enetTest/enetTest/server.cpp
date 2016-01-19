// ENet
#include <enet/enet.h>

// Protocol Buffer Files
#include "config.h"

// System Headers
#include <iostream>

int main(int argc, char* argv[])
{

  if(enet_initialize() != 0)
  {
    std::cerr << "Failed to initialize ENet." << std::endl;
    return EXIT_FAILURE;
  }

  atexit(enet_deinitialize);

  std::cout << "Hello, world!" << std::endl;
  return 0;
}