/* myserver.cc: sample server program */
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "messageHandler.h"
#include "protocol.h"
#include "newsgroup.h"
#include "article.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <algorithm>

using namespace std;


int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}

    vector<Newsgroup> newsgroups;
	
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
                messageHandler m(*conn);
                // Read message type
                unsigned char type = conn->read(); 
                switch (type) {
                    case Protocol::COM_LIST_NG:
                        m.serverListNG(newsgroups);
                        break;
                    case Protocol::COM_CREATE_NG:
                        m.serverCreateNG(newsgroups);
                        break;
                    case Protocol::COM_DELETE_NG:
                        m.serverDeleteNG(newsgroups);
                        break;
                    case Protocol::COM_LIST_ART:
                        m.serverListArt(newsgroups);
                        break;
                    case Protocol::COM_CREATE_ART:
                        m.serverCreateArt(newsgroups);
                        break;
                    case Protocol::COM_DELETE_ART:
                        m.serverDeleteArt(newsgroups);
                        break;
                    case Protocol::COM_GET_ART:
                        m.serverGetArt(newsgroups);
                        break;
                    default:
                        // throw exception
                        cerr << "If you see this, something is wrong!" << endl;
                        break;
                }
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
}
