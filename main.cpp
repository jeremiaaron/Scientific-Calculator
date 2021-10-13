#include <iostream>
#include <cctype>
#include <cmath>
#include "Lists.h"
#include "HistoryCalc.h"
using namespace std;

//--------------------------------------------------------------------------------------
//FUNCTIONS FOR EXPRESSION EVALUATION (+, -, *, /, ^, sin, cos, tan, sqrt, log, ln)

float basicOpCalculate(float x, float y, char op) {
    if(op == '+') return (x + y);
    if(op == '-') return (x - y);
    if(op == '*') return (x * y);
    if(op == '/') return (x / y);
    if(op == '^') return pow(x, y);
    return 0;
}

float advOpCalculate(float x, char op) {
    
    /*
    CHAR REPRESENTATIONS FOR TRIGONOMETRIC AND ADVANCED FUNCTIONS
    's': sin
    'c': cos
    't': tan
    'q': sqrt
    'l': log
    'e': ln
    */

    if(op == 's') return sin(x);
    if(op == 'c') return cos(x);
    if(op == 't') return tan(x);
    if(op == 'q') return sqrt(x);
    if(op == 'l') return log10(x);
    if(op == 'e') return log(x);
    return 0;
}

int opPrecedence(char op) {
    if(op == '+' || op == '-')
        return 1;
    else if (op == '*' || op == '/')
        return 2; 
    else if (op == 's' || op == 'c' || op == 't' || op == 'q' || op == 'l' || op == 'e')
        return 3;
    else if (op == '^')
        return 4;  
    else
        return 0;
}

float inputResult(string input, Vals* valsHead, Vals* valsTail, Ops* opsHead, Ops* opsTail) {
    for(int i = 0; i < input.length(); i++) {
        if(input[i] == ' ') {
            continue;
        }

        else if(input[i] == '(') {
            opsHead->pushToTail(&opsHead, &opsTail, input[i]);
        }

        else if(isalpha(input[i])) {
            if(input[i] == 's') {
                if(i < input.length() - 2 && input[i + 1] == 'i' && input[i + 2] == 'n') {
                    opsHead->pushToTail(&opsHead, &opsTail, 's');
                    i += 2;
                }
                else if(i < input.length() - 3 && input[i + 1] == 'q' && input[i + 2] == 'r' && input[i + 3] == 't') {
                    opsHead->pushToTail(&opsHead, &opsTail, 'q');
                    i += 3;
                }
            }

            else if(i < input.length() - 2 && input[i] == 'c' && input[i + 1] == 'o' && input[i + 2] == 's') {
                opsHead->pushToTail(&opsHead, &opsTail, 'c');
                i += 2;
            }

            else if(i < input.length() - 2 && input[i] == 't' && input[i + 1] == 'a' && input[i + 2] == 'n') {
                opsHead->pushToTail(&opsHead, &opsTail, 't');
                i += 2;
            }

            else if(input[i] == 'l') {
                if(i < input.length() - 2 && input[i + 1] == 'o' && input[i + 2] == 'g') {
                    opsHead->pushToTail(&opsHead, &opsTail, 'l');
                    i += 2;
                }

                else if(i < input.length() - 1 && input[i + 1] == 'n') {
                    opsHead->pushToTail(&opsHead, &opsTail, 'e');
                    i++;
                }
            }
        }

        else if(isdigit(input[i])) {
            float temp = 0;

            while(i < input.length() && isdigit(input[i])) {
                temp = (temp * 10) + (input[i] - '0');
                i++;
            }

            valsHead->pushToTail(&valsHead, &valsTail, temp);
            i--;
        }

        else if(input[i] == ')') {
            while(opsHead != NULL && opsTail->data != '(') {
                char tailOp = opsTail->data;

                if(tailOp == 's' || tailOp == 'c' || tailOp == 't' || tailOp == 'q' || tailOp == 'l' || tailOp == 'e') {
                    float x = valsTail->data;
                    valsHead->deleteTail(&valsHead, &valsTail);

                    char op = opsTail->data;
                    opsHead->deleteTail(&opsHead, &opsTail);

                    float result = advOpCalculate(x, op);
                    valsHead->pushToTail(&valsHead, &valsTail, result);
                }

                else {
                    float b = valsTail->data;
                    valsHead->deleteTail(&valsHead, &valsTail);

                    float a = valsTail->data;
                    valsHead->deleteTail(&valsHead, &valsTail);

                    char op = opsTail->data;
                    opsHead->deleteTail(&opsHead, &opsTail);

                    float result = basicOpCalculate(a, b, op);
                    valsHead->pushToTail(&valsHead, &valsTail, result);
                }
            }

            if(opsHead != NULL) opsHead->deleteTail(&opsHead, &opsTail);
        }
        
        else {
            while(opsHead != NULL && opPrecedence(input[i]) <= opPrecedence(opsTail->data)) {
                char tailOp = opsTail->data;

                if(tailOp == 's' || tailOp == 'c' || tailOp == 't' || tailOp == 'q' || tailOp == 'l' || tailOp == 'e') {
                    float x = valsTail->data;
                    valsHead->deleteTail(&valsHead, &valsTail);

                    char op = opsTail->data;
                    opsHead->deleteTail(&opsHead, &opsTail);

                    float result = advOpCalculate(x, op);
                    valsHead->pushToTail(&valsHead, &valsTail, result);
                }

                else {
                    float b = valsTail->data;
                    valsHead->deleteTail(&valsHead, &valsTail);

                    float a = valsTail->data;
                    valsHead->deleteTail(&valsHead, &valsTail);

                    char op = opsTail->data;
                    opsHead->deleteTail(&opsHead, &opsTail);

                    float result = basicOpCalculate(a, b, op);
                    valsHead->pushToTail(&valsHead, &valsTail, result);
                }
            }
    
            opsHead->pushToTail(&opsHead, &opsTail, input[i]);
        }
    }

    while(opsHead != NULL) {
        char tailOp = opsTail->data;

        if(tailOp == 's' || tailOp == 'c' || tailOp == 't' || tailOp == 'q' || tailOp == 'l' || tailOp == 'e') {
            float x = valsTail->data;
            valsHead->deleteTail(&valsHead, &valsTail);

            char op = opsTail->data;
            opsHead->deleteTail(&opsHead, &opsTail);

            float result = advOpCalculate(x, op);
            valsHead->pushToTail(&valsHead, &valsTail, result);
        }

        else {
            float b = valsTail->data;
            valsHead->deleteTail(&valsHead, &valsTail);

            float a = valsTail->data;
            valsHead->deleteTail(&valsHead, &valsTail);

            char op = opsTail->data;
            opsHead->deleteTail(&opsHead, &opsTail);

            float result = basicOpCalculate(a, b, op);
            valsHead->pushToTail(&valsHead, &valsTail, result);
        }
    }

    return valsTail->data;
}

// Changes here
// Erases `count` lines, including the current line
void eraseLines(int count) {
    count++;
    if (count > 0) {
        std::cout << "\x1b[2K"; // Delete current line
        // i=1 because we included the first line
        for (int i = 1; i < count; i++) {
            std::cout
            << "\x1b[1A" // Move cursor up one
            << "\x1b[2K"; // Delete the entire line
        }
        std::cout << "\r"; // Resume the cursor at beginning of line
    }
} // Changes here


//FUNCTION MAIN MENU
int mainMenu() {
    int menu;
    cout << "=========================" << endl;
    cout << "\tMAIN MENU\t" << endl;
    cout << "=========================" << endl << endl;
    cout << "1. Open scientific calculator" << endl;
    cout << "2. Open calculator history" << endl;
    cout << "0. Exit program" << endl << endl;
    
    cout << "Enter your option: ";
    cin >> menu;
    getchar();

    system("cls");

    return menu;
}

//DRIVER PROGRAM
int main() {
    Vals* valsHead = NULL, *valsTail = NULL;
    Ops* opsHead = NULL, *opsTail = NULL;
    Queue* history = new Queue(1000);
    
    //obtain value from user input in main menu
    int menu = mainMenu();

    while(menu != 0) {
        //SCIENTIFIC CALCULATOR MENU
        if(menu == 1) {
            cout << "=====================================" << endl;
            cout << "\tSCIENTIFIC CALCULATOR\t" << endl;
            cout << "=====================================" << endl;
            cout << "Available operators: +, -, *, /, ^, (, ), sin, cos, tan, sqrt, log, ln" << endl;
            cout << "Please enter your expression(s)! (type DONE to return to main menu)" << endl << endl;

			string input;
			int error = 0;
    		int openBracket = 0, closeBracket = 0, i = 0, firstCalc = 1;
			
		    do { // CHANGES HERE
                
                if(firstCalc == 1) {
        			firstCalc++;
				}else if(error == 1) {
           			eraseLines(3);
           			cout << "Please enter an appropriate expression" << endl;
               		error--;
				}
		    	
		    	cout <<  "Enter an expression: ";
        		getline(cin, input);
        		
   				closeBracket = 0;
       			openBracket = 0;
       			
       			for(i = 0; i < input.length(); i++) {
       				
           			if(input[i] == ' ' || input[i] == '\n') {
               			continue;
	            	}
           			else if(input[i] == '(') {
               			openBracket++;
               			continue;
           			}
           			else if(input[i] == ')') {
               			closeBracket++;
               			continue;
           			}
           			else if(isdigit(input[i])) {
               			continue;
           			}
           			else if (input[i] == '^' && isdigit(input[i+1])) {
               			continue;
           			}
           			else if(input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {
               			if(isdigit(input[i+1]) || input[i+1] == 's' || input[i+1] == 'c' || input[i+1] == 't' || input[i+1] == 'l')
                   			continue;
           			}
           			else if(input[i] == 's' && input[i+1] == 'q' && input[i+2] == 'r' && input[i+3] == 't') {
               			i += 3;
               			continue;
           			}
           			else if(input[i] == 's' && input[i+1] == 'i' && input[i+2] == 'n') {
               			i += 2;
               			continue;
           			}
           			else if(input[i] == 'c' && input[i+1] == 'o' && input[i+2] == 's') {
               			i += 2;
               			continue;
           			}
           			else if(input[i] == 't' && input[i+1] == 'a' && input[i+2] == 'n') {
               			i += 2;
               			continue;
           			}
           			else if(input[i] == 'l' && input[i+1] == 'o' && input[i+2] == 'g') {
               			i += 2;
               			continue;
           			}
           			else if(input[i] == 'l' && input[i+1] == 'n') {
               			i += 1;
               			continue;
           			}
           			else if(input == "DONE") {
           				error++;
						break;
					}
					else if(input[i] == '=') {
						error++; break;
					}else error++; break;
       			}
				
				if(openBracket != closeBracket || input[0] == ')' || input[input.length()-1] == '(' && error != 1) {
					error++;
				}
				
				if(error == 0) {
					float result = inputResult(input, valsHead, valsTail, opsHead, opsTail);
		        	cout << "= " << result << endl << endl;
		        	history->enqueue(history, input, result);
				}
			}while(input.compare("DONE") != 0);
        } //CHANGES HERE

        //HISTORY MENU
        else if(menu == 2) {
            cout << "=====================================" << endl;
            cout << "\tCALCULATOR HISTORY\t" << endl;
            cout << "=====================================" << endl;
            cout << "1. Show history list" << endl;
            cout << "2. Pick a history to delete" << endl;
            cout << "3. Delete whole history" << endl;
            cout << "0. Back to main menu" << endl << endl;

            float option;
			
			do{
				cout << "Enter your option: ";
				cin >> option;
				
				if(option == 1) {
                    history->showHistory(history);
                }

                else if(option == 2) {
                    int index;
                    cout << "Which history do you want to delete? ";
                    cin >> index;
                    history->dequeue_At(history, index);
                }

                else if(option == 3) {
                    char answer;

                    cout << "Do you want to delete the whole history (Y/N)? ";
                    cin >> answer;

                    switch(tolower(answer)) {
                        case 'y': history->dequeue_All(history);
                                  if(!history->isEmpty(history))
                                      cout << "All history successfully deleted" << endl;
                                  break;
                        case 'n': cout << "History deletion cancelled" << endl; break;
                    }

                    cout << endl;
                }
			}while(option != 0);
        }

        system("cls"); //clears current menu before moving to another menu

        menu = mainMenu();
    }

    //exits program when user enters 0
    exit(0);

    return 0;
}
