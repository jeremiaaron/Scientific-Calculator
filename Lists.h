#ifndef LISTSH
#define LISTSH
#include <stddef.h>

class Vals {
    public:
        float data;
        Vals* next;

        Vals(float x) {
            data = x;
            next = NULL;
        }

        void pushToTail(Vals** headRef, Vals** tailRef, float newData);
        void deleteTail(Vals** headRef, Vals** tailRef);
        void printList(Vals* valsHead);
};

class Ops {
    public:
        char data;
        Ops* next;

        Ops(char c) {
            data = c;
            next = NULL;
        }

        void pushToTail(Ops** headRef, Ops** tailRef, char newData);
        void deleteTail(Ops** headRef, Ops** tailRef);
        void printList(Ops* valsHead);
};

#endif