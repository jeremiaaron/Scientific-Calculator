#include <iostream>
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
};

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
Queue* createQueue(unsigned capacity)
{
	Queue* queue = new Queue();
	queue->capacity = capacity;
	queue->front = queue->size = 0;

	// This is important, see the enqueue
	queue->rear = capacity - 1;
	queue->array = new string[queue->capacity]; // Create an array with n-capacity size
	return queue;
}

// Queue is full when size
// becomes equal to the capacity
bool isFull(Queue* queue)
{
	return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
bool isEmpty(Queue* queue)
{
	return (queue->size == 0);
}

// Function to add an expression to the queue.
// It changes rear and size
void enqueue(Queue* queue, string expression, int result)
{
	
	if(queue->size == 0) { // If queue is empty, reset the front and rear to -1 again
		queue->front = -1;
		queue->rear = -1;
	}
	
	expression = expression + to_string(result); // Concatenate the string expression with the int result into one string
	
	if (isFull(queue))
		return;
	else if(queue->front == -1) { // If front is -1 add +1 so that front is pointing to the beginning of the array
		queue->front += 1;
	}
	queue->rear = (queue->rear + 1) % queue->capacity; // Modulo here is used to limit the shifting of rear from one element to another in the array
	queue->array[queue->rear] = expression; // Store the string expression into the array
	queue->size = queue->size + 1; // Increase the size as the expression is added
	cout << expression << " enqueued to queue\n" << endl;
}

// Remove an expression done in history at a given position
void dequeue_At(Queue* queue, int pos) {
		
	int i;
	int count = 0;

	if (isEmpty(queue)) {
		cout << "\tError! Empty History\t\n" << endl;
		return;
	}
	
	for(i = pos-1; i < (queue->size-1); i++) { // Loop from the deleted element to shift all the following elements on the right where the element being deleted is located to the left so no gap is present
    	queue->array[i] = queue->array[i+1];
	}
	queue->rear = (queue->rear - 1) % queue->capacity; // Decrease rear by one because an element is deleted
	queue->size = queue->size - 1; // Decrease size by one too
	count++;
      	
    if(queue->front == queue->rear) { // If front and rear is pointng at the same array element, assign -1 to both
		queue->front = -1;
		queue->rear = -1;
		return;
	}
	if(count == 0) {
    	cout<<"History not found...\n" << endl;
    	return;
	}else 	cout<<"History deleted successfully...\n" << endl;
}

// Remove all expressions from queue
void dequeue_All(Queue* queue) {
	if (isEmpty(queue)) {
		cout << "\tError! Empty History\t\n" << endl;
		return;
	}
	
	// Loop through the array and basically have the rear "pointer" go back all the way to the front "pointer"
	for(int i = 0; i < queue->size; i++) {
		queue->rear = (queue->rear - 1) % queue->capacity; // Modulo here is used to limit the shifting of front from one element to another in the array
		queue->size = queue->size - 1;
		
		if(queue->front == queue->rear) { // If front and rear is pointng at the same array element, assign -1 to both and decrease size by -1 so that the size will be zero
			queue->size = queue->size - 1;
			queue->front = -1;
			queue->rear = -1;
			return;
		}
	}
}

void showHistory(Queue* queue) {
	
	cout << "|-------------------CALCULATOR HISTORY-------------------|\n" << endl;
	
	// Check if the array is empty
	if(isEmpty(queue)) {
		cout << "\t\t\tEmpty History\t\n" << endl;
	}else {
		for(int i = 0; i < queue->size; i++) {
			cout << queue->array[i] << "\n" << endl;		
		}
	}
	cout << "|---------------------------END--------------------------|\n" << endl;
	return;
}


// Function to get front of queue
/*int front(Queue* queue)
{
	if (isEmpty(queue))
		return INT_MIN;
	return queue->array[queue->front];
}

// Function to get rear of queue
int rear(Queue* queue)
{
	if (isEmpty(queue))
		return INT_MIN;
	return queue->array[queue->rear];
}*/

/*  C++ Program to Delete an Element in an array  */

// Driver code
int main()
{
	Queue* queue = createQueue(1000);

	enqueue(queue, "2+3*5 = ", 10);
	enqueue(queue, "5*4/23-3 = ", 2);
	enqueue(queue, "14%4-5+8*2 = ", 15);
	enqueue(queue, "7-2+11*4%3 = ", -2);

	showHistory(queue);

	dequeue_At(queue, 4);
	
	showHistory(queue);
	
	dequeue_At(queue, 1);
	
	showHistory(queue);
	
	dequeue_At(queue, 2);
	
	showHistory(queue);
	
	dequeue_At(queue, 1);
	
	showHistory(queue);

	dequeue_All(queue);
	
	cout << "The current size is "<< queue->size << endl;
	
	showHistory(queue);
	
	enqueue(queue, "5*6/12 = ", 15);
	
	showHistory(queue);
	
	enqueue(queue, "23%2+6 = ", 10);
	
	enqueue(queue, "23%2+6 = ", 10);

	showHistory(queue);
	
	dequeue_All(queue);
	
	showHistory(queue);

	/*cout << dequeue(queue)
		<< " dequeued from queue\n";*/

	/*cout << "Front expression is "
		<< front(queue) << endl;
	cout << "Rear expression is "
		<< rear(queue) << endl;*/

	return 0;
}
