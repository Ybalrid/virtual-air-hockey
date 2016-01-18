#include <iostream>
#include <string.h>
#include <enet/enet.h>
#include "config.h"


using namespace std;

char buffer[BUFFERSIZE];
ENetHost *client;
ENetAddress address;
ENetEvent event;
ENetPeer *peer;
ENetPacket *packet;


int main (int argc, char ** argv) 
{
    int connected = 0;

	if (argc != 1) {
		cout << "Usage : client username " << endl;
		exit;
	}

	if (enet_initialize() != 0) {
		cout << "Could not initialize enet." << endl;
		return 0;
	}

	client = enet_host_create(NULL, 1, 2, 5760/2, 1440/8);

	if (client == NULL) {
		cout << "Could not create client." << endl;
		return 0;
	}

	enet_address_set_host(&address, HOST);
	address.host = PORT;

	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL) {
		cout << "Could not connect to server" << endl;
		return 0;
	}

	if (enet_host_service(client, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		cout << "Connection to %s succeeded." << HOST << endl;
		connected++;

		strncpy(buffer, argv[1], BUFFERSIZE);
		packet = enet_packet_create(buffer, strlen(buffer)+1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	} else {
		enet_peer_reset(peer);
		cout << "Could not connect to %s" << HOST << endl;
		return 0;
	}

	while (1) {
		while (enet_host_service(client, &event, 1000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_RECEIVE:
				puts((char*) event.packet->data);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				connected = 0;
				cout << "You have been disconnected" << endl;
				return 2;
			}
		}

		if (connected) {
			cout << "Input: ";
			gets(buffer);

			if (strlen(buffer) == 0) { continue; }

			if (strncmp("q", buffer, BUFFERSIZE) == 0) {
				connected = 0;
				enet_peer_disconnect(peer, 0);
				continue;
			}

			packet = enet_packet_create(buffer, strlen(buffer)+1, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet);
		}
	}

	enet_deinitialize();

}
