#include <iostream>
#include <enet/enet.h>

using namespace std;

int main (int argc, char ** argv) 
{
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    atexit (enet_deinitialize);

	cin.get();
}
