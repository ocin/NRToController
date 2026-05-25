#ifndef COMMON_H
#define COMMON_H

class TocDelegate;
class DCCEXProtocol;;
class WiFiClient;

//#define DEBUG 1 

#define ENABLE_SERIAL 1

#define TU_CLOSE 0
#define TU_THROWN 1
#define TU_UNKNOWN 2

extern WiFiClient client;
extern DCCEXProtocol dccexProtocol;
extern TocDelegate tocDelegate;

#endif