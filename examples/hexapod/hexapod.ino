//#include "hexapod_controller.hpp"

//Hexapod hexapod; 

void setup() {

  Serial.begin(115200);
  Serial4.begin(115200); //Raspberry Pi Serial

}

void loop() {
  double pos;
  uint8_t leg_or_action;
  uint8_t motor;
  uint8_t speed;
  hexapod.runSpeed();

  if (Serial.available() > 0 || Serial4.available() > 0) {
    String command;
    if (Serial.available() > 0) {
      command = Serial.readStringUntil('\n');
    } else {
      command = Serial4.readStringUntil('\n');
    }

    sscanf(command.c_str(), "%hu %hu %lf %hu", &leg_or_action, &motor, &pos, &speed);

    //temp switch for user interaction
    switch(leg_or_action) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
        Serial.printf("leg: %d; motor: %d; pos: %f\n", leg_or_action, motor, pos);
        Serial4.printf("leg: %d; motor: %d; pos: %f\n", leg_or_action, motor, pos);
        //hexapod.moveLegToPos(leg, motor, pos);
	//hexapod.legs[leg - 1].axes[motor - 1].moveToPosAtSpeed(pos, speed);
	hexapod.moveLegToPosAtSpeed(leg, motor, pos, speed);
        break;
      case 7:
        Serial.printf("Zeroing all legs\n");
        Serial4.printf("Zeroing all legs\n");
        //hexapod.moveToZeros();
        break;
      case 8:
        Serial.printf("Hexapod standing\n");
        Serial4.printf("Hexapod standing\n");
        //hexapod.stand();
        break;
      case 9:
        Serial.printf("Hexapod sitting\n");
        Serial4.printf("Hexapod sitting\n");
        //hexapod.sit();
        break;
      default:
        Serial.printf("Invalid Input\n");
        Serial4.printf("Invalid Input\n");
        break;
    }
	}
}
