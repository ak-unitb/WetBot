/**
 * subset of the sketch provided by arduino.cc for Bridge usage:
 * for more explanations @see http://arduino.cc/en/Tutorial/Bridge
 */

#include <YunServer.h>
 
 
// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
YunServer server;

void initYunServer();
void listenApiRequests();
void processApiRequest(YunClient client);
void digitalCommand(YunClient client);
