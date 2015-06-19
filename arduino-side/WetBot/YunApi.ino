/**
 * subset of the sketch provided by arduino.cc for Bridge usage:
 * for more explanations @see http://arduino.cc/en/Tutorial/Bridge
 */

#include <YunClient.h>

void initYunServer() {
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}

void listenApiRequests() {
  YunClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    processApiRequest(client);

    // Close connection and free resources.
    client.stop();
  }
}

// we just accept digital read or write operations ...
void processApiRequest(YunClient client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "digital" command?
  if (command == "digital") {
    digitalCommand(client);
  }
}

void digitalCommand(YunClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (client.read() == '/') {
    value = client.parseInt();
    //Serial.print("digitalCommand: Write: pin: ");
    //Serial.print(pin);
    //Serial.print(" value: ");
    //Serial.println(value);
    digitalWrite(pin, value);
    // Send feedback to client
    client.print(pin);
    client.print(" set to ");
    client.println(value);
  } else {
    //Serial.print("digitalCommand: Read: pin: ");
    //Serial.print(pin);
    value = digitalRead(pin);
    client.println(value);
  }


  // Update datastore key with the current pin value
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}
