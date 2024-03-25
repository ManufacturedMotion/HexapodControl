//#include "hexapod_controller.hpp"

//Hexapod hexapod; 

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

    uint8_t leg_or_action;
    uint8_t motor;
    double pos;
    int num_parsed = sscanf(command.c_str(), "%hhu %hhu %lf", &leg_or_action, &motor, &pos);
    
    if (num_parsed == 3 && (leg_or_action >= 1 && leg_or_action <= 6)) {
      switch (leg_or_action) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
          Serial.printf("leg: %d; motor: %d; pos: %f\n", leg_or_action, motor, pos);
          Serial4.printf("leg: %d; motor: %d; pos: %f\n", leg_or_action, motor, pos);
          //hexapod.moveLegToPos(leg, motor, pos);
          break;
      }
    } else if (num_parsed == 1) {
      switch (leg_or_action) {
        case 7:
          Serial.println("Zeroing all legs");
          Serial4.println("Zeroing all legs");
          //hexapod.moveToZeros();
          break;
        case 8:
          Serial.println("Hexapod standing");
          Serial4.println("Hexapod standing");
          //hexapod.stand();
          break;
        case 9:
          Serial.println("Hexapod sitting");
          Serial4.println("Hexapod sitting");
          //hexapod.sit();
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
