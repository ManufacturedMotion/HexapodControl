#include "hexapod_controller.hpp"
#include "hexapod.hpp"
#include "FIFOCommandQueue.hpp"
#include <math.h>
#include <stdbool.h>
Hexapod hexapod;

#if DEBUG
#define SERIAL_OUTPUT Serial
#else
#define SERIAL_OUTPUT Serial4
#endif

const int bufferSize = 64;
String buffer[bufferSize];
String split_command[bufferSize];
uint32_t num_words = 0;
double x = 0, y = 0, z = 200, roll = 0, pitch = 0, yaw = 0, speed = 100;
bool wait = false;

Position position;
FIFOCommandQueue fifo;

void setup() {
  Serial.begin(115200);
  Serial4.begin(115200);
}

void loop() {

  String command = "";
  String fifo_command = "";

  if (Serial.available() > 0 || Serial4.available() > 0) {
    if (Serial4.available() > 0) {
      command = Serial4.readStringUntil('\n');
    } else {
      command = Serial.readStringUntil('\n');
    }
    SERIAL_OUTPUT.print("Teensy Received: " + command + ".\n");
    fifo.enqueue(command);
  }

  if (!fifo.isEmpty()) {

    if (hexapod.isBusy() || wait) {

    } else {
      //set wait to default of fasle & get the command from the fifo after checking for optimizations
      wait = false;
      fifo_command = getFifoCommand();
      executeCommand(fifo_command);
    } 
  }
  //call move every iteration of loop
  hexapod.linearMovePerform();
}

//function to split string on specified substring
void splitString(String command, String sub_string, String split_words[], uint32_t& num_words) {
  uint32_t start_index = 0;
  int16_t substring_index = 0;
  uint32_t word_index = 0;

  // Initialize split_words array with empty strings
  for (uint32_t i = 0; i < bufferSize; i++) {
    split_words[i] = "";
  }

  command.toLowerCase();
  sub_string.toLowerCase();

  while (true) {
    substring_index = command.indexOf(sub_string, start_index);
    if (substring_index == -1) {
      split_words[word_index] = command.substring(start_index);
      break;
    }
    split_words[word_index] = command.substring(start_index, substring_index);
    start_index = substring_index + sub_string.length();  // Move the start index past the substring
    word_index++;
  }

  num_words = word_index + 1;
}

//update x y z roll pitch yaw, etc
void updateVariables(String current_command_substring) {

  if (current_command_substring.startsWith('x')) {
    splitString(current_command_substring, 'X', buffer, num_words);
    String x_str = buffer[1];
    x = x_str.toFloat();
  } else if (current_command_substring.startsWith('y')) {
    splitString(current_command_substring, 'Y', buffer, num_words);
    String y_str = buffer[1];
    y = y_str.toFloat();
  } else if (current_command_substring.startsWith('z')) {
    splitString(current_command_substring, 'Z', buffer, num_words);
    String z_str = buffer[1];
    z = z_str.toFloat();
  } else if (current_command_substring.startsWith('r')) {
    splitString(current_command_substring, 'R', buffer, num_words);
    String roll_str = buffer[1];
    roll = roll_str.toFloat();
  } else if (current_command_substring.startsWith('p')) {
    splitString(current_command_substring, 'P', buffer, num_words);
    String pitch_str = buffer[1];
    pitch = pitch_str.toFloat();
  } else if (current_command_substring.startsWith('w')) {
    splitString(current_command_substring, 'W', buffer, num_words);
    String yaw_str = buffer[1];
    yaw = yaw_str.toFloat();
  } else if (current_command_substring.startsWith('s')) {
    splitString(current_command_substring, 'S', buffer, num_words);
    String speed_str = buffer[1];
    speed = speed_str.toFloat();
  } else if (current_command_substring.startsWith('h')) {
    splitString(current_command_substring, 'H', buffer, num_words);
    wait = buffer[1].toInt();
  }
}

//execute a command string
void executeCommand(String command) {

  //split on spaces
  splitString(command, ' ', split_command, num_words);
  uint32_t cmd_line_word_count = 0;
  cmd_line_word_count = num_words;

  //G-code commands
  if (split_command[0].startsWith('g')) {
    splitString(split_command[0], 'G', buffer, num_words);
    if (!buffer[1].equals("0") and !buffer[1].equals("1")) {
      SERIAL_OUTPUT.printf("Error: only G0 and G1 implemented.\n");
    } else {
      String current_command_substring;
      if (buffer[1].equals("0")) {
        for (uint8_t i = 0; i < cmd_line_word_count; i++) {
          current_command_substring = split_command[i];
          updateVariables(current_command_substring);
          position.set(x, y, z, roll, pitch, yaw);
        }
        SERIAL_OUTPUT.printf("rapid move parsing success; x, y, z is %f, %f, %f\n roll, pitch, yaw, speed are %f, %f, %f, %f.\n", x, y, z, roll, pitch, yaw, speed);
        hexapod.rapidMove(position);
      }
      else if (buffer[1].equals("1")) {
        for (uint8_t i = 0; i < cmd_line_word_count; i++) {
          current_command_substring = split_command[i];
          updateVariables(current_command_substring);
          position.set(x, y, z, roll, pitch, yaw);
        }
        SERIAL_OUTPUT.printf("linear move parsing success; x, y, z is %f, %f, %f\n roll, pitch, yaw, speed are %f, %f, %f, %f.\n", x, y, z, roll, pitch, yaw, speed);
        hexapod.linearMoveSetup(position, speed);
      }
    }
  }
  else if (split_command[0].startsWith('p')) {
    splitString(split_command[0], 'P', buffer, num_words);
    if (buffer[1] == "0") {
      SERIAL_OUTPUT.printf("parsing success; starfish preset selected (move all motors to zero).\n");
      hexapod.moveToZeros();
      return;
    }
    else {
      SERIAL_OUTPUT.printf("parser detected input for a preset that is not yet supported.\n");
    }
  }
  else {
    SERIAL_OUTPUT.printf("Unsupported input recieved.\n");
  }
}


//get the next fifo command, if possible perform any optimizations before returning the command
String getFifoCommand() {
  String fifo_command = fifo.dequeue();
  String command_type = getCommandType(fifo_command);
  if (command_type == "step") {
    //if fifo is still getting commands but the next command is not a step, we will execute the current step regardless of size (no optimization can be made)
    //TODO - do we actually care if fifo is idle? Logic would be the same regardless right? seems dangerous to force a wait until idle - what if we are never idle?
    //if (!fifo.isIdle()) {
      if (fifo.length > 0) {
        String next_command_type = getCommandType(fifo.readNext());
        if (next_command_type != "step") {
          return fifo_command;
        }
        //otherwise the next command is a step; check if our current command exceeds step threshold. If not, we can combine steps until we have met the threshold or the next command is not a step 
        else {
          TODO - get distance for current step
          String current_command = fifo_command;
          String next_command = fifo.readNext();
          while ((step_size < STEP_THRESHOLD) and !fifo.isEmpty() and (getCommandType(next_command) == "step")) {
            fifo_command = combineSteps(fifo_command, next_command);
            TODO - same update to step_size as above
            current_command = fifo.dequeue();
            next_command = fifo.readNext();
          }
          return fifo_command;
        }
      }
    //}
// copy/paste/edit for debug SERIAL_OUTPUT.print("Teensy Received: " + command + ".\n");
  return fifo_command;
}


//TODO - do we need to combine roll pitch yaw speed or wait differently?
String combineSteps(String step_1, String step_2) {
  double x_1 = 0, y_1 = 0, z_1 = 0, roll_1 = 0, pitch_1 = 0, yaw_1= 0, speed_1 = 0;
  double x_2 = 0, y_2 = 0, z_2 = 0, roll_2 = 0, pitch_2 = 0, yaw_2= 0, speed_2 = 0;
  bool wait1 = 0; wait2 = 0;  
  x_1 = step_1.substring(step_1.indexOf('X') + 1).toFloat();
  x_2 = step_2.substring(step_2.indexOf('X') + 1).toFloat();
  y_1 = step_1.substring(step_1.indexOf('Y') + 1).toFloat();
  y_2 = step_2.substring(step_2.indexOf('Y') + 2).toFloat();
  z_1 = step_1.substring(step_1.indexOf('Z') + 1).toFloat();
  z_2 = step_2.substring(step_2.indexOf('Z') + 2).toFloat();
  roll_1 = step_1.substring(step_1.indexOf('R') + 1).toFloat();
  roll_2 = step_2.substring(step_2.indexOf('R') + 2).toFloat();
  pitch_1 = step_1.substring(step_1.indexOf('P') + 1).toFloat();
  pitch_2 = step_2.substring(step_2.indexOf('P') + 2).toFloat();
  yaw_1 = step_1.substring(step_1.indexOf('W') + 1).toFloat();
  yaw_2 = step_2.substring(step_2.indexOf('W') + 2).toFloat();
  speed_1 = step_1.substring(step_1.indexOf('S') + 1).toFloat();
  speed_2 = step_2.substring(step_2.indexOf('S') + 2).toFloat();
  wait_1 = step_1.substring(step_1.indexOf('H') + 1).toInt();
  wait_2 = step_2.substring(step_2.indexOf('H') + 2).toInt();
  String new_step = "G1";
  double new_x = x_1 + x_2; 
  double new_y = y_1 + y_2;
  double new_z = z_1 + z_2;
  double new_roll = (roll_1 + roll_2) / 2;
  double new_pitch = (pitch_1 + pitch_2) / 2;
  double new_yaw = (yaw_1 + yaw_2) / 2;
  double new_speed = (speed_1 + speed_2) / 2;
  bool new_wait = wait_1 or wait_2;
  new_step += " X" + String(new_x) + " Y" + String(new_y) + " Z" + String(new_z) + " R" + String(new_roll) + " P" + String(new_pitch) + " W" + String(new_yaw) + " S" + String(new_speed) + " H" + String(new_wait);
  return new_step;
}

//check the first index of the command and return a string summarizing the type
String getCommandType(String command) {
  String ret_val = "unknown";
  splitString(command, ' ', split_command, num_words);
  if (split_command[0] == "G1") {
   ret_val = "step"; 
  }
  else if (split_command[0].startsWith('p')) {
   ret_val = "preset";
  }
  return ret_val;
}

