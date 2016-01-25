#include <enet/enet.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <conio.h>
using namespace std;

int main(int argc, char **argv)
{
	cout << "Enter 1(server) or 2(client)" << endl;
	int choice;
	cin >> choice;
	cout << endl << endl;

	if (choice==1) {
		ENetAddress address;
		ENetHost *server;
		ENetEvent event;
		ENetPeer *peer = NULL;
		int eventStatus;

		// a. Initialize enet
		if (enet_initialize() != 0) {
			fprintf(stderr, "An error occured while initializing ENet.\n");
			return EXIT_FAILURE;
		}

		atexit(enet_deinitialize);

	
		// b. Create a host using enet_host_create
		address.host = ENET_HOST_ANY;
		address.port = 1234;

		server = enet_host_create(&address, 32, 2, 0, 0);

		if (server == NULL) {
			fprintf(stderr, "An error occured while trying to create an ENet server host\n");
			exit(EXIT_FAILURE);
		}
		else cout << "Server up and running (type 'q' to quit)" << endl << endl;

		// c. Connect and user service
		eventStatus = 1;
		enet_uint32 wait = 60000;
		bool first = true;

		while (1) {
			eventStatus = enet_host_service(server, &event, wait);

			// If we had some event that interested us
			if (eventStatus > 0) {
				switch(event.type) {
					case ENET_EVENT_TYPE_CONNECT:
						printf("(Server) We got a new connection from %x\n",
								event.peer->address.host);
						peer = event.peer;
						wait = 0;
						break;

					case ENET_EVENT_TYPE_RECEIVE:
						printf("(Client>%s\n", event.packet->data);
						// Lets broadcast this message to all
						//enet_host_broadcast(server, 0, event.packet);
						break;

					case ENET_EVENT_TYPE_DISCONNECT:
						printf("%s disconnected.\n", event.peer->data);
						// Reset client's information
						event.peer->data = NULL;
						break;

				}
			}

			printf("Server> ");
			string message = "hello";
			if (first) {
				getline(cin,message);
				first = false;
			}
			getline(cin,message);
			if (message=="q") return 0;

			if (strlen(message.c_str()) > 0) {
				ENetPacket *packet = enet_packet_create(message.c_str(), strlen(message.c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer, 0, packet);
			}
		}
	}

	if (choice==2) {
		ENetAddress address;
		ENetHost *client;
		ENetPeer *peer;
		//char message[1024];
		ENetEvent event;
		int eventStatus;

		// a. Initialize enet
		if (enet_initialize() != 0) {
			fprintf(stderr, "An error occured while initializing ENet.\n");
			return EXIT_FAILURE;
		}

		atexit(enet_deinitialize);

		// b. Create a host using enet_host_create
		client = enet_host_create(NULL, 1, 2, 57600/8, 14400/8);

		if (client == NULL) {
			fprintf(stderr, "An error occured while trying to create an ENet server host\n");
			exit(EXIT_FAILURE);
		}

		enet_address_set_host(&address, "localhost");
		address.port = 1234;

		// c. Connect and user service
		peer = enet_host_connect(client, &address, 2, 0);

		if (peer == NULL) {
			fprintf(stderr, "No available peers for initializing an ENet connection");
			exit(EXIT_FAILURE);
		}

		eventStatus = 1;
		bool first = true;
		int wait = 1000;

		
		while (1) {
			eventStatus = enet_host_service(client, &event, 0);

			// If we had some event that interested us
			if (eventStatus > 0) {
				switch(event.type) {
					case ENET_EVENT_TYPE_CONNECT:
						printf("(Client) We got a new connection from %x\n",
								event.peer->address.host);
						wait = 0;
						break;

					case ENET_EVENT_TYPE_RECEIVE:
						//printf("(Client) Message from server : %s\n", event.packet->data);
						printf("Server>%s\n", event.packet->data);
						// Lets broadcast this message to all
						// enet_host_broadcast(client, 0, event.packet);
						enet_packet_destroy(event.packet);
						break;

					case ENET_EVENT_TYPE_DISCONNECT:
						printf("(Client) %s disconnected.\n", event.peer->data);
						// Reset client's information
						event.peer->data = NULL;
						break;
				}
			}

			printf("Client> ");
			string message;
			if (first) {
				getline(cin,message);
				first = false;
			}
			getline(cin,message);
			if (message=="q") return 0;

			if (strlen(message.c_str()) > 0 && peer) {
				ENetPacket *packet = enet_packet_create(message.c_str(), strlen(message.c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer, 0, packet);
			}
		}
	}
} 