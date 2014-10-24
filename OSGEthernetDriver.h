// 
// File:   OSGEthernetDriver.h
// Author: geoff
//
// Created on October 13, 2013, 2:56 PM
//

#ifndef _OSGETHERNETDRIVER_H
#define	_OSGETHERNETDRIVER_H

#include <Arduino.h>
#include <SPI.h>
#include <EthernetV2_0.h>
#include <stdint.h>

class OSGEthernetDriver
{
protected:
    EthernetUDP *Udp;
    int *udp_broadcast; 

    EthernetClient client;   
    
public:
    OSGEthernetDriver() {
        
        //clear the pins for the sd card so the ethernet can work
        pinMode(SDCARD_CS_PIN, OUTPUT);
        digitalWrite(SDCARD_CS_PIN, HIGH);
       

        //ethernet settings
        byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
        ip = IPAddress(192,168,1,75);
        gateway = IPAddress(192,168,1, 1);
        subnet  = IPAddress(255, 255, 255, 0);
        server = IPAddress(192,168,1,187);
        dns1 = IPAddress(4,4,4,4);
        state = STATE_DISCONNECTED;
        
#ifdef DEBUG
        Serial.println("Ethernet Initialized...");
#endif
       // Udp = new EthernetUDP();
       // Udp->begin(LOCAL_PORT);
    }

    void loop() {
        switch (state) {
            case STATE_DISCONNECTED:
                
                /*for now we have to assume that since we don't have dhcp to tell us
                  that we're 'connected' to the ethernet (no return values to tell us 'status')
                */ 
                this->connect();
                state = STATE_CONNECTED;
            break;

            case STATE_CONNECTED:

            break;
        }
    }

    EthernetUDP* getUdp() {
        return Udp;
    }
    
    int* getUdpBroadcast() {
        return udp_broadcast;
    }
    
    IPAddress& getServer() {
        return server;
    }


private:
    char *header;
    int state;

    byte mac[6];
    IPAddress ip;
    IPAddress dns1;
    IPAddress gateway;
    IPAddress subnet; 
    IPAddress server;

    void connect() {
        //a progress report
        #ifdef DEBUG
            Serial.println("Starting Ethernet...");
        #endif

        //TODO: Get DHCP working.
        Ethernet.begin(mac, ip, dns1, gateway, subnet);

        // if(Ethernet.begin(mac) == 0) {
        //     Serial.println(F("DHCP FAILED"));
        //     Ethernet.begin(mac, ip, dns1, gateway, subnet);
        // }
        
        delay (1000);
        #ifdef DEBUG
            Serial.print("My IP address: ");
        

            for (byte thisByte = 0; thisByte < 4; thisByte++) {
                // print the value of each byte of the IP address:
                Serial.print(Ethernet.localIP()[thisByte], DEC);
                Serial.print("."); 

            }
            Serial.println();
        #endif
    } 


    
    
    
    
};
#endif	/* _OSGETHERNETDRIVER_H */

