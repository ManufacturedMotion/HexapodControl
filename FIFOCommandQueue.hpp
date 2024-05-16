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
      String readNext();
		  _Bool isEmpty();
      _Bool isIdle();
		  uint32_t length = 0;
    private:
      uint64_t _last_enqueue_timestamp = 0;
		
};

#endif
