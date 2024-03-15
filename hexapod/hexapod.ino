#include "hexapod_controller.hpp"

Hexapod hexapod; 

void setup() {

  Serial.begin(115200);

}

void loop() {
  double pos;
  uint8_t leg;
  uint8_t motor;

  	if (Serial.available() > 0) {
    	String command = Serial.readStringUntil('\n');
    	Serial.println("Received\n");
    	sscanf(command.c_str(), "%hu %hu %lf", &leg, &motor, &pos);

		//temp switch for user interaction
		switch(leg) {
			default:
				break;
			case 0:
				Serial.printf("Instructed to set to zeros");
				hexapod.moveToZeros();
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				Serial.printf("leg: %d; motor: %d; pos: %f", leg, motor, pos);
				hexapod.moveLegToPos(leg, motor, pos);
				break;
			case 7:
				//sit placehold
				break;
			case 8:
				hexapod.stand();
				break;
  		}
	}
}
