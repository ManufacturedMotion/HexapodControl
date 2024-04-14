#include <stdbool.h>
#include <Arduino.h>
#include "FIFOCommandQueue.hpp"


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
