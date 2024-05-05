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
  SERIAL_OUTPUT.begin(115200);
}

void loop() {

  String command = "";
  String command_from_fifo = "";
  uint32_t cmd_line_word_count = 0;

  if (SERIAL_OUTPUT.available() > 0) {
    command = SERIAL_OUTPUT.readStringUntil('\n');
    fifo.enqueue(command);
  }

  if (!fifo.isEmpty()) {

    if (hexapod.isBusy() || wait) {

    } else {
      command_from_fifo = fifo.dequeue();
      wait = false;

      //split on spaces
      splitString(command_from_fifo, ' ', split_command, num_words);
      cmd_line_word_count = num_words;

      //G-code commands
      if (split_command[0].startsWith('g')) {

        splitString(split_command[0], 'G', buffer, num_words);
        if (!buffer[1].equals("0") and !buffer[1].equals("1")) {
          SERIAL_OUTPUT.printf("Error: only G0 and G1 implemented");
        } else {

          String current_command_substring;

          if (buffer[1].equals("0")) {

            for (uint8_t i = 0; i < cmd_line_word_count; i++) {

              current_command_substring = split_command[i];
              updateVariables(current_command_substring);
              position.set(x, y, z, roll, pitch, yaw); 
                
            }

            SERIAL_OUTPUT.printf("rapid move parsing success; x, y, z is %f, %f, %f\n roll, pitch, yaw, speed are %f, %f, %f, %f\n", x, y, z, roll, pitch, yaw, speed);
            hexapod.rapidMove(position);

          }

          else if (buffer[1].equals("1")) {

            for (uint8_t i = 0; i < cmd_line_word_count; i++) {

              current_command_substring = split_command[i];
              updateVariables(current_command_substring);
              position.set(x, y, z, roll, pitch, yaw); 
            }

            SERIAL_OUTPUT.printf("linear move parsing success; x, y, z is %f, %f, %f\n roll, pitch, yaw, speed are %f, %f, %f, %f\n", x, y, z, roll, pitch, yaw, speed);
            hexapod.linearMoveSetup(position, speed);
          }
        }
      }

      else if (split_command[0].startsWith('p')) {
        splitString(split_command[0], 'P', buffer, num_words);
        if (buffer[1] == "0") {

          SERIAL_OUTPUT.printf("parsing success; starfish preset selected (move all motors to zero)\n");
          hexapod.moveToZeros();
          return;

        }

        else {

          SERIAL_OUTPUT.printf("parser detected input for a preset that is not yet supported");
        }

      }

      else {

        SERIAL_OUTPUT.printf("Unsupported input recieved");
      }
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
