#ifndef HISTORYCALCH
#define HISTORYCALCH
#include <string>
using namespace std;

// A structure to represent a queue
class Queue {
public:
	int front = -1;
	int rear = -1;
	int size; // Size of elements filled into the array
	unsigned capacity; // Size of array
	string* array; // Create a pointer array
	
	// constructor to create a queue
	// of given capacity.
	// It initializes size of queue as 0
	Queue(unsigned capacity) {
		this->capacity = capacity;
		this->front = size = 0;

		// This is important, see the enqueue
		this->rear = capacity - 1;
		this->array = new string[capacity]; // Create an array with n-capacity size
	}

	bool isFull(Queue* queue);
	bool isEmpty(Queue* queue);
	void enqueue(Queue* queue, string expression, float result);
	void dequeue_At(Queue* queue, int pos);
	void dequeue_All(Queue* queue);
	void showHistory(Queue* queue);
};

#endif