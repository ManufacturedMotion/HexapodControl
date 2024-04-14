_Bool FIFOCommandQueue::enqueue(String str_command) {
    Command new_command(str_command);
    if (head == NULL) {
        head = &new_command;
    }
    else {
        tail->next = &new_command;
    }
    tail = &new_command;
}

String FIFOCommandQueue::dequeue(String str_command) {
    if (head != NULL) {
        String ret_string = head->command;
        head = head->next;
        return ret_string;
    }
    else {
        return String("");
    }
}

Command::Command(String str_command) {
    command = str_command;
}