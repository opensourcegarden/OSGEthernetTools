/* 
 * File:   OSGUDPPacketWrapper.h
 * Author: root
 *
 * Created on October 17, 2013, 8:25 PM
 */

#ifndef OSGUDPPACKETWRAPPER_H
#define	OSGUDPPACKETWRAPPER_H
#include "OSGEthernetDriver.h"
#include <SPI/SPI.h>
#include <EthernetV2_0/EthernetV2_0.h>
#include <EthernetV2_0/EthernetUdpV2_0.h>
#include <stdint.h>

#define BUFFER_SIZE 128

const char header[] = "OSG/1.0\n";

class OSGUDPPacketWrapper {
public:
    
    OSGUDPPacketWrapper(OSGEthernetDriver *eth0) {
      this->eth0 = eth0;  
      udp = eth0->getUdp();
    };
    
    void sendMessage(IPAddress address, char* send_message) {
        char data[1024];
        strcpy(data, header);

        
        udp->beginPacket(address,8102);
        
        udp->write(data);
        udp->write(send_message);
        udp->endPacket();
    }
    
    char* receiveMessage() {
        
        int packetSize = udp->available();
        Serial.print(F("Received packet of size "));
        Serial.println(packetSize);
        Serial.print(F("From "));
        IPAddress remote = udp->remoteIP();
        for (int i =0; i < 4; i++)
        {
          Serial.print(remote[i], DEC);
          if (i < 3)
          {
            Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(udp->remotePort());  
        udp->read(receive_message,128);
        // Serial.println("Contents:");
        // Serial.println(receive_message);  
    }


private:
    EthernetUDP *udp;
    OSGEthernetDriver *eth0;
    char *send_message;
    char receive_message[128];
};


#endif	/* OSGUDPPACKETWRAPPER_H */

