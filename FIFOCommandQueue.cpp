#include <stdbool.h>
#include <Arduino.h>
#include "FIFOCommandQueue.hpp"


void FIFOCommandQueue::enqueue(String str_command) {
    Serial.println("enque: command is");
	Serial.println(str_command);
	Command* new_command = new Command(str_command);
	if (head == NULL) {
        head = new_command;
    }
    else {
        tail->next = new_command;
    }
    tail = new_command;
	Serial.println(head->command);
	length++;
}

String FIFOCommandQueue::dequeue() {
    if (head != NULL) {
        String ret_string = head->command;
		Serial.println("deququ:");
		Serial.println(ret_string);
        Command* temp = head;
		head = head->next;
		delete temp;
		length--;
        return ret_string;
    }
    else {
		Serial.println("womp womp");
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

}
