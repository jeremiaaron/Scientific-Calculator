#include "Lists.h"
#include <iostream>
using namespace std;

void Vals::pushToTail(Vals** headRef, Vals** tailRef, float newData) {
    Vals* newNode = new Vals(newData);

    if(*headRef == NULL) 
        *headRef = newNode;
    else
        (*tailRef)->next = newNode;

    (*tailRef) = newNode;
}

void Ops::pushToTail(Ops** headRef, Ops** tailRef, char newData) {
    Ops* newNode = new Ops(newData);

    if(*headRef == NULL) 
        *headRef = newNode;
    else
        (*tailRef)->next = newNode;

    (*tailRef) = newNode;
}

void Vals::deleteTail(Vals** headRef, Vals** tailRef) {
    if(*headRef == NULL)
        return;
    
    if((*headRef)->next == NULL) {
        (*headRef) = (*headRef)->next;
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

void Ops::deleteTail(Ops** headRef, Ops** tailRef) {
    if(*headRef == NULL)
        return;
    
    if((*headRef)->next == NULL) {
        (*headRef) = (*headRef)->next;
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

void Vals::printList(Vals* valsHead) {
    while(valsHead != NULL) {
        cout << valsHead->data << " ";
        valsHead = valsHead->next;
    }
}

void Ops::printList(Ops* opsHead) {
    while(opsHead != NULL) {
        cout << opsHead->data << " ";
        opsHead = opsHead->next;
    }
}