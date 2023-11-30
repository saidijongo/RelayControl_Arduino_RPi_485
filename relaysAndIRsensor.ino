#include <Arduino.h>

const int RELAY_PIN[] = {2, 6, 7, 8};
const int IR_SENSOR_PIN = 3;
int previousIRState = LOW;

void setup() {
  Serial.begin(115200);
  Serial.flush();

  for (int i = 0; i < sizeof(RELAY_PIN) / sizeof(RELAY_PIN[0]); i++) {
    pinMode(RELAY_PIN[i], OUTPUT);
    digitalWrite(RELAY_PIN[i], LOW);
  }

  pinMode(IR_SENSOR_PIN, INPUT);
  previousIRState = digitalRead(IR_SENSOR_PIN);
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void relayControl(String relayCommand) {
  String relayName = getValue(relayCommand, ',', 2);
  int relayState = getValue(relayCommand, ',', 3).toInt();

  int relayPinIndex = -1;

  if (relayName.equals("PRINTER")) {
    relayPinIndex = 0;
  } else if (relayName.equals("DISPENSER")) {
    relayPinIndex = 1;
  } else if (relayName.equals("CARD_READER")) {
    relayPinIndex = 2;
  } else if (relayName.equals("PASSPORT_READER")) {
    relayPinIndex = 3;
  } else {
    Serial.print("ST,0," + relayName + ",ERR_INVALID_COMMAND,ED\r\n");
    return;
  }

  // Check and update the relay state
  if (relayState == 1 && digitalRead(RELAY_PIN[relayPinIndex]) == LOW) {
    // Turn ON (HIGH) only if it's currently OFF (LOW)
    digitalWrite(RELAY_PIN[relayPinIndex], HIGH);
  } else if (relayState == 0 && digitalRead(RELAY_PIN[relayPinIndex]) == HIGH) {
    // Turn OFF (LOW) only if it's currently ON (HIGH)
    digitalWrite(RELAY_PIN[relayPinIndex], LOW);
  }

  // Send the current relay state in the response
  Serial.print("ST,0," + relayName + "," + String(digitalRead(RELAY_PIN[relayPinIndex])) + ",ED\r\n");
}

void loop() {
  int currentIRState = digitalRead(IR_SENSOR_PIN);

  if (currentIRState != previousIRState) {
    // IR Sensor state changed
    Serial.print("ST,0,IRSENSOR," + String(currentIRState) + ",ED\r\n");
    previousIRState = currentIRState;
  }

  if (Serial.available() > 0) {
    String data = Serial.readString();
    relayControl(data);
  }
}
