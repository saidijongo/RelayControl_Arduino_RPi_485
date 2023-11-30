
#include <Arduino.h>

const int RELAY_PIN[] = {2, 6, 7, 8};

void setup() {
  Serial.begin(115200);
  Serial.flush();

  for (int i = 0; i < sizeof(RELAY_PIN) / sizeof(RELAY_PIN[0]); i++) {
    pinMode(RELAY_PIN[i], OUTPUT);
    digitalWrite(RELAY_PIN[i], LOW); // LOW initially
  }
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
  int relayState = getValue(relayCommand, ',', 4).toInt();

  if (relayName.equals("PRINTER") && relayState == 1) {
    digitalWrite(RELAY_PIN[0], HIGH);
    Serial.print("ST,0," + relayName + ",OK," + String(relayState) + ",ED\r\n");
  } else if (relayName.equals("PRINTER") && relayState == 0) {
    digitalWrite(RELAY_PIN[0], LOW);
    Serial.print("ST,0," + relayName + ",OK," + String(relayState) + ",ED\r\n");
  } else if (relayName.equals("DISPENSER") && relayState == 1) {
    digitalWrite(RELAY_PIN[1], HIGH);
    Serial.print("ST,0," + relayName + ",OK," + String(relayState) + ",ED\r\n");
  } else if (relayName.equals("DISPENSER") && relayState == 0) {
    digitalWrite(RELAY_PIN[1], LOW);
    Serial.print("ST,0," + relayName + ",OK," + String(relayState) + ",ED\r\n");
  } else if (relayName.equals("CARD_READER") && relayState == 1) {
    digitalWrite(RELAY_PIN[2], HIGH);
    Serial.print("ST,0," + relayName + ",OK," + String(relayState) + ",ED\r\n");
  } else if (relayName.equals("CARD_READER") && relayState == 0) {
    digitalWrite(RELAY_PIN[2], LOW);
    Serial.print("ST,0," + relayName + ",OK," + String(relayState) + ",ED\r\n");
  } else if (relayName.equals("PASSPORT_READER") && relayState == 1) {
    digitalWrite(RELAY_PIN[3], HIGH);
    Serial.print("ST,0," + relayName + ",OK," + String(relayState) + ",ED\r\n");
  } else if (relayName.equals("PASSPORT_READER") && relayState == 0) {
    digitalWrite(RELAY_PIN[3], LOW);
    Serial.print("ST,0," + relayName + ",OK," + String(relayState) + ",ED\r\n");
  } else {
    Serial.print("ST,0," + relayName + ",ERR_INVALID_COMMAND,ED\r\n");
  }
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readString();
    relayControl(data);
  }
}
