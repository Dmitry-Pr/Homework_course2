#include "iostream"

struct message {
    int senderType;
    int messageType;
    int whomToSend;
    char text[1024]; // Фиксированный размер строки
};