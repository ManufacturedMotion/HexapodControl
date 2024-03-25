void setup() {
  Serial.begin(115200);
  Serial4.begin(115200); // Raspberry Pi Serial
}

void loop() {
  if (Serial.available() > 0 || Serial4.available() > 0) {
    String command;
    if (Serial.available() > 0) {
      command = Serial.readStringUntil('\n');
    } else {
      command = Serial4.readStringUntil('\n');
    }

    // Parse command
    uint8_t leg_or_action;
    uint8_t motor;
    double pos;
    int parsed = sscanf(command.c_str(), "%hhu %hhu %lf", &leg_or_action, &motor, &pos);
    
    // Check for valid commands
    if (parsed == 3 && (leg_or_action >= 1 && leg_or_action <= 6)) {
      // Process commands requiring three values
      switch (leg_or_action) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
          Serial.printf("leg: %d; motor: %d; pos: %f\n", leg_or_action, motor, pos);
          Serial4.printf("leg: %d; motor: %d; pos: %f\n", leg_or_action, motor, pos);
          // Perform action
          break;
      }
    } else if (parsed == 1) {
      // Process commands requiring only one value
      switch (leg_or_action) {
        case 7:
          Serial.println("Zeroing all legs");
          Serial4.println("Zeroing all legs");
          // Perform action
          break;
        case 8:
          Serial.println("Hexapod standing");
          Serial4.println("Hexapod standing");
          // Perform action
          break;
        case 9:
          Serial.println("Hexapod sitting");
          Serial4.println("Hexapod sitting");
          // Perform action
          break;
        default:
          Serial.println("Invalid Input");
          Serial4.println("Invalid Input");
          break;
      }
    } else {
      // Invalid command format
      Serial.println("Invalid command format");
      Serial4.println("Invalid command format");
    }
  }
}
