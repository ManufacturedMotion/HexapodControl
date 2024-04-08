#include "hexapod_controller.hpp"
#include <math.h>
Hexapod hexapod; 

void setup() {

  Serial.begin(115200);

}

void loop() {
  
    double x = 0, y = 0, z = 200, roll = 0, pitch = 0, yaw = 0, speed = 100;
	String command;

    if (Serial.available() > 0 || Serial4.available() > 0) {
		if (Serial.available() > 0) {
   			String command = Serial.readStringUntil('\n');
   			Serial.println("Received\n");
		}
		else {
			String command = Serial.readStringUntil('\n');
			Serial4.println("Received\n");
		}

		String split_command[16];
		uint32_t num_words = 0;
		
		//split on spaces
		splitString(command, ' ', split_command, num_words);

		String buffer[16];

		//G-code commands		
		if (split_command[0].startsWith('g')) {
	
			splitString(split_command[0], 'G', buffer, num_words);
			if(!buffer[1].equals("1")){
				Serial.println("Error only G1 implemented");
				Serial4.println("Error only G1 implemented");
			}
			else {
        		String current_command_substring;
				
				for(uint8_t i = 1; i < 15; i++) {
					current_command_substring = split_command[i].toLowerCase();

					if (current_command_substring.startsWith('x')) {
						splitString(current_command_substring, 'X', buffer, num_words);
                		String x_str = buffer[1]; float x_float = x_str.toFloat(); x = (double)x_float;
					}
					else if (current_command_substring.startsWith('y')) {
						splitString(current_command_substring, 'Y', buffer, num_words);
                		String y_str = buffer[1]; float y_float = y_str.toFloat(); y = (double)y_float;
					}        		
					else if (current_command_substring.startsWith('z')) {
						splitString(current_command_substring, 'Z', buffer, num_words);
                		String z_str = buffer[1]; float z_float = z_str.toFloat(); z = (double)z_float;
					}
					else if (current_command_substring.startsWith('r')) {
						splitString(current_command_substring, 'R', buffer, num_words);
                		String roll_str = buffer[1]; float roll_float = roll_str.toFloat(); roll = (double)roll_float;
					}
					else if (current_command_substring.startsWith('p')) {
						splitString(current_command_substring, 'P', buffer, num_words);
                		String pitch_str = buffer[1]; float pitch_float = pitch_str.toFloat(); pitch = (double)pitch_float;
					}
					else if (current_command_substring.startsWith('w')) {
						splitString(current_command_substring, 'W', buffer, num_words);
                		String yaw_str = buffer[1]; float yaw_float = yaw_str.toFloat(); yaw = (double)yaw_float;
					}
					else if (current_command_substring.startsWith('s')) {
						splitString(current_command_substring, 'S', buffer, num_words);
                		String speed_str = buffer[1]; float speed_float = speed_str.toFloat(); speed = (double)speed_float;
					}

				}	

        		Serial.printf("parsing success; x, y, z is %f, %f, %f\n roll, pitch, yaw, speed are\n", x, y, z, roll, pitch, yaw, speed);
        		Serial4.printf("parsing success; x, y, z is %f, %f, %f\n roll, pitch, yaw, speed are\n", x, y, z, roll, pitch, yaw, speed);
				hexapod.linearMoveSetup(x, y, z, roll, pitch, yaw, speed);
	
      		}
		}
		
		else if (split_command[0].startsWith('p')) {
			splitString(split_command[0], 'P', buffer, num_words);	
			if (buffer[1] == "0") {
			
				Serial.printf("parsing success; using Zero preset\n");
				Serial4.printf("parsing success; using Zero preset\n");
				hexapod.moveToZeros();

			}
			
			else {
	
				Serial.printf("parser detected preset input of %s. This preset not yet supported. \n", buffer[1]);
				Serial4.printf("parser detected preset input of %s. This preset not yet supported. \n", buffer[1]);
	
			}

		}


    	else {

      		Serial.printf("recieved input from controller \n"); 
     		Serial4.printf("recieved input from controller \n"); 
      		printArray(split_command, 16);

    	}

	}
	hexapod.runSpeed();
  	hexapod.linearMovePerform();
}

//function to split string on specified substring
void splitString(String command, String sub_string, String split_words[], uint32_t& num_words) {
    uint32_t start_index = 0;
    int16_t substring_index = 0;
    uint32_t word_index = 0;

	// Initialize split_words array with empty strings
    for (uint32_t i = 0; i < 16; i++) {
        split_words[i] = "";
    }

	command.toLowerCase();
	sub_string.toLowerCase();
	
    while(true) {
        substring_index = command.indexOf(sub_string, start_index);
        if(substring_index == -1){
            split_words[word_index] = command.substring(start_index);
            break;
        }
        split_words[word_index] = command.substring(start_index, substring_index);
        start_index = substring_index + sub_string.length(); // Move the start index past the substring
        word_index++;
    }

    num_words = word_index + 1;
}

//print buffers for debug
void printArray(String array[], uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        Serial.print(array[i]);
        Serial4.print(array[i]);
        Serial.print(" "); 
        Serial4.print(" "); 
    }
    Serial.println(); 
    Serial4.println(); 
}

