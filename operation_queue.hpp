#include <stdbool.h>
#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "three_by_matrices.hpp"

#ifndef OPERATION_QUEUE
#define OPERATION_QUEUE

class Operation {
    public:
        Operation(ThreeByOne op_end_pos, double op_speed, _Bool op_relative);
        ThreeByOne end_pos = ThreeByOne(0.0, 0.0, 0.0);
        double speed;
        _Bool relative;
        Operation * next;
    private:
};

class OperationQueue {
    public:
		Operation * head = NULL;
        Operation * tail = NULL;
        void enqueue(ThreeByOne op_end_pos, double op_speed, _Bool op_relative);
        void dequeue();
		_Bool isEmpty();
		uint32_t length = 0;
    private:
		
};

#endif
