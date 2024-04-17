#include <stdbool.h>
#include <Arduino.h>
#include <stdint.h>

#ifndef fifo_command
#define fifo_command

class Command {
    public:
        Command(String command);
        String command;
        Command * next;
    private:
};

class FIFOCommandQueue {
    public:
        FIFOCommandQueue();
		Command * head;
        Command * tail;
        void enqueue(String str_command);
        String dequeue();
		_Bool isEmpty();
		uint32_t length = 0;
    private:
		
};

#endif
