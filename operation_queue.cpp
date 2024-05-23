#include <stdbool.h>
#include "operation_queue.hpp"

void OperationQueue::enqueue(ThreeByOne op_end_pos, double op_speed, _Bool op_relative) {
	Operation* new_operation = new Operation(op_end_pos, op_speed, op_relative);
	if (head == NULL) {
        head = new_operation;
		head->next = NULL;
    }
    else {
        tail->next = new_operation;
    }
    tail = new_operation;
	tail->next = NULL;
	length++;
}

void OperationQueue::dequeue() {
	if (head != NULL) {
        Operation* temp = head;
		head = temp->next;
		delete temp;
		length--;
    }
}

Operation::Operation(ThreeByOne op_end_pos, double op_speed, _Bool op_relative) {
    end_pos = ThreeByOne(op_end_pos);
    speed = op_speed;
    relative = op_relative;
}

_Bool OperationQueue::isEmpty(){
	return head == NULL;
}

