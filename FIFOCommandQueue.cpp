#include <stdbool.h>
#include <Arduino.h>
#include "FIFOCommandQueue.hpp"
#include "config.hpp"

void FIFOCommandQueue::enqueue(String str_command) {
	Command* new_command = new Command(str_command);
	if (head == NULL) {
        head = new_command;
		head->next = NULL;
    }
    else {
        tail->next = new_command;
    }
    tail = new_command;
	tail->next = NULL;
	length++;
  _last_enqueue_timestamp = millis();
}

String FIFOCommandQueue::dequeue() {
	if (head != NULL) {
        String ret_string = head->command;
        Command* temp = head;
		head = temp->next;
		delete temp;
		length--;
        return ret_string;
    }
    else {
        return String("");
    }
}

String FIFOCommandQueue::readNext() {
  if (head != NULL) {
      String ret_string = head->command;
      return ret_string
  }
  else {
    return String("");
  }
}

Command::Command(String str_command) {
    command = str_command;
}

_Bool FIFOCommandQueue::isEmpty(){
	return head == NULL;
}

FIFOCommandQueue::FIFOCommandQueue() {

	head = NULL;
	tail = NULL;
  _last_enqueue_timestamp = 0;

}

_Bool FIFOCOmmandQueue::isIdle() {

  if ((millis() - _last_enqueue_timestamp) > FIFO_IDLE_THRESHOLD) { 
    return True;
  else {
    return False;
  }
}
}
