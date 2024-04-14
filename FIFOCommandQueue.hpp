class FIFOCommandQueue {
    public:
        FIFOCommandQueue
        Command * head;
        Command * tail;
        _Bool enqueue(String str_rommand);
        _Bool dequeue(uint8_t position);
    private:
};

class Command {
    public:
        Command(String command);
        String command;
        Command * next;
    private:
}