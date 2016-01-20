#include <enet/enet.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "config.h"
#pragma warning(disable:4996)

class Host
{
    ENetAddress address;
    ENetHost * server;
    ENetHost* client;
    ENetEvent event;
public:
    Host()
        :server(NULL)
    {
        enet_initialize();
        setupServer();
    }

    void setupServer()
    {
        if(server)
        {
            enet_host_destroy(server);
            server = NULL;
        }

        address.host = ENET_HOST_ANY;
        /* Bind the server to port 1234. */
        address.port = 1721;

        server = enet_host_create (& address /* the address to bind the server host to */, 
            32      /* allow up to 32 clients and/or outgoing connections */,
            2      /* allow up to 2 channels to be used, 0 and 1 */,
            0      /* assume any amount of incoming bandwidth */,
            0      /* assume any amount of outgoing bandwidth */);

    }

     void daLoop()
     {
         while(true)
         {
             /* Wait up to 1000 milliseconds for an event. */
             while (enet_host_service (server, & event, 5000) > 0)
             {
                 ENetPacket * packet;

                 switch (event.type)
                 {
                 case ENET_EVENT_TYPE_CONNECT:
                     printf ("A new client connected from %x:%u.\n", 
                         event.peer -> address.host,
                         event.peer -> address.port);

                     /* Store any relevant client information here. */
                     event.peer -> data = "Client information";

                     /* Create a reliable packet of size 7 containing "packet\0" */
                     packet = enet_packet_create ("packet", 
                         strlen ("packet") + 1, 
                         ENET_PACKET_FLAG_RELIABLE);

                     /* Extend the packet so and append the string "foo", so it now */
                     /* contains "packetfoo\0"                                      */
                     enet_packet_resize (packet, strlen ("packetfoo") + 1);
                     strcpy ((char*)& packet -> data [strlen ("packet")], "foo");

                     /* Send the packet to the peer over channel id 0. */
                     /* One could also broadcast the packet by         */
                     /* enet_host_broadcast (host, 0, packet);         */
                     enet_peer_send (event.peer, 0, packet);
                     /* One could just use enet_host_service() instead. */
                     enet_host_flush (server);

                     break;

                 case ENET_EVENT_TYPE_RECEIVE:
                     printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                         event.packet -> dataLength,
                         event.packet -> data,
                         event.peer -> data,
                         event.channelID);

                     /* Clean up the packet now that we're done using it. */
                     enet_packet_destroy (event.packet);

                     break;

                 case ENET_EVENT_TYPE_DISCONNECT:
                     printf ("%s disconected.\n", event.peer -> data);

                     /* Reset the peer's client information. */

                     event.peer -> data = NULL;
                 }
             }
         }

     }

        ~Host()
    {
        if(server)
        {
            enet_host_destroy(server);
            server = NULL;
        }

        atexit (enet_deinitialize);
    }
};

class Client
{
    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;
    ENetHost* client;
public:
    Client()
        :peer(NULL)
    {
        enet_initialize();
        setupPeer();
    }

    void setupPeer()
    {

        client = enet_host_create (NULL /* create a client host */,
            1 /* only allow 1 outgoing connection */,
            2 /* allow up 2 channels to be used, 0 and 1 */,
            57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
            14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

        if (client == NULL)
        {
            fprintf (stderr, 
                "An error occurred while trying to create an ENet client host.\n");
            exit (EXIT_FAILURE);
        }

        /* Connect to some.server.net:1234. */
        enet_address_set_host (& address, HOST);
        address.port = 1721;

        /* Initiate the connection, allocating the two channels 0 and 1. */
        peer = enet_host_connect (client, & address, 2, 0);    

        if (peer == NULL)
        {
            fprintf (stderr, 
                "No available peers for initiating an ENet connection.\n");
            exit (EXIT_FAILURE);
        }

        /* Wait up to 5 seconds for the connection attempt to succeed. */
        if (enet_host_service (client, & event, 20000) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT)
        {
            std::cout << "Connection to some.server.net:1234 succeeded." << std::endl;
        }
        else
        {
            /* Either the 5 seconds are up or a disconnect event was */
            /* received. Reset the peer in the event the 5 seconds   */
            /* had run out without any significant event.            */
            enet_peer_reset (peer);

            puts ("Connection to some.server.net:1234 failed.");
        }
    }


    void daLoop()
    {
        ENetPacket* packet;

        /* Create a reliable packet of size 7 containing "packet\0" */
        packet = enet_packet_create ("backet", 
            strlen ("backet") + 1, 
            ENET_PACKET_FLAG_RELIABLE);

        /* Extend the packet so and append the string "foo", so it now */
        /* contains "packetfoo\0"                                      */
        enet_packet_resize (packet, strlen ("backetfoo") + 1);
        strcpy ((char*)& packet -> data [strlen ("backet")], "foo");

        /* Send the packet to the peer over channel id 0. */
        /* One could also broadcast the packet by         */
        /* enet_host_broadcast (host, 0, packet);         */
        enet_peer_send (event.peer, 0, packet);
        /* One could just use enet_host_service() instead. */
        enet_host_flush (client);

        while(true)
        {
            /* Wait up to 1000 milliseconds for an event. */
            while (enet_host_service (client, & event, 1000) > 0)
            {
                ENetPacket * packet;

                switch (event.type)
                {
                case ENET_EVENT_TYPE_RECEIVE:
                    printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                        event.packet -> dataLength,
                        event.packet -> data,
                        event.peer -> data,
                        event.channelID);

                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy (event.packet);

                    break;
                }
            }
        }
    }
    ~Client()
    {
        atexit (enet_deinitialize);
    }
};

int main()
{
    std::string a;
    std::cin >> a;
    if(a == "host")
    {
        Host host;
        host.daLoop();
    }
    else
    {
        Client c;
        c.daLoop();
    }

    return 0;
}