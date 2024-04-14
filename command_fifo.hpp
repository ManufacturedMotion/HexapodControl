#include <stdbool.h>

#ifndef command_fifo
#define command_fifo

	struct Command {
	
    	String command;
    	Command* next_command;
    	Command(String command): command(command), Command(nullptr) {}

	}

	class CommandFifo {

    	private:
        	Command* head;
        	Command* tail;

    	public:
        	CommandFifo(): head(nullptr), tail(nullptr) {}

        	~CommandFifo() {
        	while (head != nullptr) {
            	Command* temp = head;
            	head = head->next;
            	delete temp;
            	}
        	}


			void addCommand(String new_command){

				Command* newCommand = new Command(new_command);
        		if (tail == nullptr) {
            		head = newCommand;
            		tail = newCommand;
        		} else {
            		tail->next = newCommand;
            		tail = newCommand;
        		}
    		}
		
			
			String popCommand() {

        		if (head == nullptr) {
            		std::cerr << "Buffer underflow\n";
            		return -1; // Or throw an exception
        		}

        		String command = head->command;
        		Command* temp = head;
        		head = head->next_command;
        		delete temp;

        		if (head == nullptr) {
            		tail = nullptr; // Update tail if list becomes empty
       			}

        		return command;
    		}		
		
			bool isEmpty() {

				return head == nullptr;				

			}
	
	}

#endif
