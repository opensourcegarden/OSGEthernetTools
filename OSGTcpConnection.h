#include <EthernetClientV2_0.h>
#include "../OSGArduino/OSGCommandParser.h"

#ifndef OSGTCPCONNECTION_H
#define OSGTCPCONNECTION_H

#define PING_DELAY 1000

#define TCP_STATE_DISCONNECTED 0
#define TCP_STATE_CONNECTED 1
#define TCP_STATE_CHECK_RECEIVE 2
#define TCP_STATE_CHECK_SEND 3 
#define TCP_STATE_PARSE_MESSAGE 4

#define STATUS_PIXEL 0



class OSGTcpConnection {
private:
	EthernetClient client;
	OSGCommandParser* parser;
	IPAddress server_ip;
	int port;
	int state;
	int last_ping_time;
	// NeoPixelStrip* strip1;
	uint8_t read_buffer[128];
	char write_buffer[128];
	NeoPixelController* pixel_controller;
public:
	OSGTcpConnection() {
		Serial.println(freeMemory());
		client = EthernetClient();

		server_ip = IPAddress(192,168,1,187);
		port = 8102;
		state = TCP_STATE_DISCONNECTED;
		last_ping_time = 0;
		pixel_controller = NeoPixelController::getInstance();
		//strip1 = new NeoPixelStrip(12, 9);
		// strip1->rainbow(1);
	}


	void loop() {
		// this->strip1->rainbow(1);
		
		// Serial.print("Strip 1: ");
		// Serial.println((int) strip1);

		// Serial.print("State: ");
		// Serial.println(state);

		switch(state) {
			case TCP_STATE_DISCONNECTED:
				{	
					/* turn the 1st pixel red */
					(pixel_controller->getPixel(STATUS_PIXEL))->setPixelColor(255,0,0);
					Serial.println(F("Attempting tcp connection.."));
					(pixel_controller->getPixel(STATUS_PIXEL))->setPixelColor(255,255,0);
					if (client.connect(server_ip, 8102)) {
						(pixel_controller->getPixel(STATUS_PIXEL))->setPixelColor(0,255,0);
					  	Serial.println("connected: ");
					  	Serial.println(client.connected());
					  	state = TCP_STATE_CONNECTED;
					} 
					else {
						// if you didn't get a connection to the server:
						Serial.println("connection failed");
					}
				}
			break;

			case TCP_STATE_CONNECTED: {
				
				/*	if the client available bytes in the buffer
					set the state machine to check receive for the next
					iteration. 
				*/
				// Serial.print("Client Available: ");
				// Serial.println(client.available());
				
				if(client.available() != 0) {
					state = TCP_STATE_CHECK_RECEIVE;
					break;
				} 

				// else if (strlen(write_buffer) > 0) {
				// 	state = STATE_CHECK_SEND;
				// }
			}
			break;

			case TCP_STATE_CHECK_RECEIVE: {
				int buffer_status = client.available();
				client.read(read_buffer, buffer_status);

				for(int n = buffer_status; n < 128; n++) {
					read_buffer[n] = 0;
				}

				// Serial.println("Message:");
				// Serial.print((char*) read_buffer);

				state = TCP_STATE_PARSE_MESSAGE;
				
			break;
			}

			case TCP_STATE_PARSE_MESSAGE: {

				// send some dev info back to the computer
				Serial.print("Parsing Message:");
				Serial.println((char*) read_buffer);

				char* pch;
				char* read_buffer_p;
				char command_string[128];
					
				//read_buffer_p is the line that we are currently processing ("\n\0");
				read_buffer_p = strtok((char*) read_buffer, "\n\0");
					
				while(read_buffer_p != NULL) {
					
					//output more dev functionality
					Serial.print("current line: ");
					Serial.print(read_buffer_p);
					Serial.print("   Length: ");
					Serial.println(strlen(read_buffer_p));

					//copy command string from the buffer into memory
					strncpy (command_string, read_buffer_p, strlen(read_buffer_p)+1);
					Serial.print("Command_string: ");
					Serial.println(command_string);

					Serial.print("Free Memory: ");
					Serial.println(freeMemory());
					Serial.println("___________________");



					pch = strtok(command_string, ":");
					

					


					if (strcmp(pch, "OSG/1.0") == 0)  {
						Serial.println("OSG Formatted Packet Found:");
						pch = strtok(NULL, ":");
						
						String* startline = new String(pch);
						
						startline->trim();
						char startline_[128];
						startline->toCharArray(startline_,(int) strlen(pch) + 1, 0);
						delete startline;
						Serial.print("Startline_: ");
						Serial.println(startline_);
						Serial.print("Free Memory: ");
						Serial.println(freeMemory());

						// Serial.println(pch);
						if(strcmp(startline_, "SYN;") == 0) {
							client.println(F("OSG/1.0:ACK;\n\0"));
							// strip1->rainbow(1000000);
						} else if (strcmp(startline_, "PING") == 0) {
							client.println("OSG/1.0:PONG\n\0");
						} else if (strcmp(startline_, "COMMAND") == 0) {
							pch = strtok(NULL, ":");
							
							//make it a string so you can trim it
							String* module = new String(pch);
							module->trim();

							char module_[128];
							module->toCharArray(module_,(int) strlen(pch) + 1, 0);

							delete module;

							pch = strtok(NULL, "");

							parser->parseCommand(module_, pch);

							Serial.println("here");
							

							//NeoPixelController->parseCommand(pch);
						}


					}


 
							
					// 		

					// 	}

						
					// }

					//next command line
					read_buffer_p = strtok(NULL, "\n\0");
					Serial.println((char*) read_buffer);
					Serial.print("ReadBufffer_p: ");
					Serial.println(read_buffer_p);

					if (strcmp(read_buffer_p, "") == 0) {
						Serial.println(" \"\" read_buffer_p");
					} else if (read_buffer_p == NULL) {
						Serial.println(" NULL read_buffer_p");
					}
				};

				
				// pch = strtok(command_string, "\n\0");

				// Serial.print(F("Command finished, more?"));

				// for (int i = 0; i < 128; i++) {
				// 	read_buffer[i] = 0;
				// }
				client.flush();
				state = TCP_STATE_CONNECTED;
			}
			break;

			// case STATE_PING: {
			// 	client.write("PING");
			// break; }
		}
	}




};
#endif /* OSGTCPCONNECTION_H */