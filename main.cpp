#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include <iomanip>
using namespace std;

class Vals {
    public:
        float data;
        Vals* next;

        Vals(float x) {
            data = x;
            next = NULL;
        }
};

class Ops {
    public:
        char data;
        Ops* next;

        Ops(char c) {
            data = c;
            next = NULL;
        }
};

void pushToTailVal(Vals** headRef, Vals** tailRef, float newData) {
    Vals* newNode = new Vals(newData);

    if(*headRef == NULL) 
        *headRef = newNode;
    else
        (*tailRef)->next = newNode;

    (*tailRef) = newNode;
}

void pushToTailOp(Ops** headRef, Ops** tailRef, char newData) {
    Ops* newNode = new Ops(newData);

    if(*headRef == NULL) 
        *headRef = newNode;
    else
        (*tailRef)->next = newNode;

    (*tailRef) = newNode;
}

void deleteTailVal(Vals** headRef, Vals** tailRef) {
    if(*headRef == NULL)
        return;
    
    if((*headRef)->next == NULL) {
        (*headRef) = (*headRef)->next;
        free(*headRef);
        return;
    }

    Vals* temp = (*headRef);
    while(temp->next->next != NULL) {
        temp = temp->next;
    }

    free(temp->next);

    temp->next = NULL;

    (*tailRef) = temp;
}

void deleteTailOp(Ops** headRef, Ops** tailRef) {
    if(*headRef == NULL)
        return;
    
    if((*headRef)->next == NULL) {
        (*headRef) = (*headRef)->next;
        free(*headRef);
        return;
    }

    Ops* temp = (*headRef);
    while(temp->next->next != NULL) {
        temp = temp->next;
    }

    free(temp->next);

    temp->next = NULL;

    (*tailRef) = temp;
}

void printListVal(Vals* valsHead) {
    while(valsHead != NULL) {
        cout << valsHead->data << " ";
        valsHead = valsHead->next;
    }
}

void printListOp(Ops* opsHead) {
    while(opsHead != NULL) {
        cout << opsHead->data << " ";
        opsHead = opsHead->next;
    }
}

//--------------------------------------------------------------------------------------

float opCalculate(float x, float y, char op) {
    if(op == '+') return ((float)x + (float)y);
    if(op == '-') return ((float)x - (float)y);
    if(op == '*') return ((float)x * (float)y);
    if(op == '/') return ((float)x / (float)y);
    return 0;
}

int opPrecedence(char op) {
    if(op == '+' || op == '-')
        return 1;
    else if (op == '*' || op == '/')
        return 2;
    else
        return 0;
}

float inputResult(string input, Vals* valsHead, Vals* valsTail, Ops* opsHead, Ops* opsTail) {
    for(int i = 0; i < input.length(); i++) {
        if(input[i] == ' ' || input[i] == '\n') {
            continue;
        }

        else if(input[i] == '(') {
            pushToTailOp(&opsHead, &opsTail, input[i]);
        }

        else if(isdigit(input[i])) {
            float temp = 0;

            while(i < input.length() && isdigit(input[i])) {
                temp = (temp * 10) + (input[i] - '0');
                i++;
            }

            pushToTailVal(&valsHead, &valsTail, temp);
            i--;
        }

        else if(input[i] == ')') {
            while(opsHead != NULL && opsTail->data != '(') {
                float b = valsTail->data;
                deleteTailVal(&valsHead, &valsTail);

                float a = valsTail->data;
                deleteTailVal(&valsHead, &valsTail);

                char op = opsTail->data;
                deleteTailOp(&opsHead, &opsTail);

                float result = opCalculate(a, b, op);
                pushToTailVal(&valsHead, &valsTail, result);
            }

            if(opsHead != NULL) deleteTailOp(&opsHead, &opsTail);
        }
        
        else {
            while(opsHead != NULL && opPrecedence(input[i]) <= opPrecedence(opsTail->data)) {
                float b = valsTail->data;
                deleteTailVal(&valsHead, &valsTail);

                float a = valsTail->data;
                deleteTailVal(&valsHead, &valsTail);

                char op = opsTail->data;
                deleteTailOp(&opsHead, &opsTail);

                float result = opCalculate(a, b, op);
                pushToTailVal(&valsHead, &valsTail, result);
            }
    
            pushToTailOp(&opsHead, &opsTail, input[i]);
        }
    }

    while(opsHead != NULL) {
        float b = valsTail->data;
        deleteTailVal(&valsHead, &valsTail);

        float a = valsTail->data;
        deleteTailVal(&valsHead, &valsTail);

        char op = opsTail->data;
        deleteTailOp(&opsHead, &opsTail);

        float result = opCalculate(a, b, op);
        pushToTailVal(&valsHead, &valsTail, result);
    }

    return valsTail->data;
}

int main() {
    Vals* valsHead = NULL, *valsTail = NULL;
    Ops* opsHead = NULL, *opsTail = NULL;
    
    string input;
    getline(cin, input);
    cout << "The result is " << inputResult(input, valsHead, valsTail, opsHead, opsTail) << endl;

    return 0;
}
