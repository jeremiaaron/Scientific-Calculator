#include <iostream>
//#include <cctype>
#include <cmath>
#include "Lists.h"
#include "HistoryCalc.h"
using namespace std;

//--------------------------------------------------------------------------------------
//FUNCTIONS FOR EXPRESSION EVALUATION (+, -, *, /, ^, sin, cos, tan, sqrt, log, ln)

//calculate using basic operators
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

//Erases 'count' lines, including current line
void eraseLines(int count) {
    count++;

    if (count > 0) {
        cout << "\x1b[2K"; //delete current line

        for (int i = 1; i < count; i++) {
            cout
            << "\x1b[1A" //move up one line
            << "\x1b[2K"; //delete current line
        }

        cout << "\r"; //move cursor to beginning of line
    }
}

void eraseWarning(string input) {
    cout << "\x1b[1A" << "\x1b[2K";
    cout << "\x1b[1A" << "\x1b[2K";
    cout << "\x1b[1A" << "\x1b[2K";

    cout << "\nEnter an expression:" << endl;
    cout << input << endl;
    
    cout << "\r";
}

float inputResult(string input, Vals* valsHead, Vals* valsTail, Ops* opsHead, Ops* opsTail) {
    bool lastIsDigit = false;

    for(int i = 0; i < input.length(); i++) {
        //skipping whitespaces
        if(input[i] == ' ') {
            continue;
        }
        
        //pushing front brackets into ops list
        else if(input[i] == '(') {
            opsHead->pushToTail(&opsHead, &opsTail, input[i]);
            lastIsDigit = false;
        }

        //pushing sin, cos, tan, sqrt, log, ln into ops list
        else if(isalpha(input[i])) {
            lastIsDigit = false;

            if(input[i] == 's') {
                // First, check if there exist two more characters after 's' to make sure that 's' is not alone
                // and check if the following characters are 'i' and 'n'
                if(i < input.length() - 2 && input[i + 1] == 'i' && input[i + 2] == 'n') {
                    opsHead->pushToTail(&opsHead, &opsTail, 's');
                    i += 2;
                }
                else if(i < input.length() - 3 && input[i + 1] == 'q' && input[i + 2] == 'r' && input[i + 3] == 't') {
                    opsHead->pushToTail(&opsHead, &opsTail, 'q');
                    i += 3;
                }
                // returns nan if neither conditional statements are fulfilled
                else return NAN;
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
                else return NAN;
            }

            else return NAN;
        }

        //pushing number value into list
        else if(isdigit(input[i])) {
            float temp = 0;

            // To check if the number has units, tens, hundreds, or thousands, etc
            while(i < input.length() && isdigit(input[i])) {
                temp = (temp * 10) + (input[i] - '0');
                i++;
            }
            i--;

            // What does this part do?
            if(!lastIsDigit && (opsHead != NULL || valsHead != NULL)) {
                if(opsTail->data == '+') {
                    opsHead->deleteTail(&opsHead, &opsTail);
                    valsHead->pushToTail(&valsHead, &valsTail, temp);
                }
                else if(opsTail->data == '-') {
                    opsHead->deleteTail(&opsHead, &opsTail);
                    valsHead->pushToTail(&valsHead, &valsTail, (-1) * temp);
                }
                else valsHead->pushToTail(&valsHead, &valsTail, temp);
            }

            else valsHead->pushToTail(&valsHead, &valsTail, temp);
            
            lastIsDigit = true;
        }

        //calculating inside parentheses
        else if(input[i] == ')') {
            while(opsHead != NULL && opsTail->data != '(') {
                char tailOp = opsTail->data;

                if(valsHead == NULL || ((tailOp == '+' || tailOp == '-' || tailOp == '*' || tailOp == '/' || tailOp == '^') && valsHead->next == NULL)) {
                    return NAN;
                }

                else if(tailOp == 's' || tailOp == 'c' || tailOp == 't' || tailOp == 'q' || tailOp == 'l' || tailOp == 'e') {
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
        
        //pushing + - * / ^ into ops list (calculates previous if current ops' precedence is lower/equal)
        else {
            if(opsHead != NULL && lastIsDigit) {
                while(opsHead != NULL && opPrecedence(input[i]) <= opPrecedence(opsTail->data)) {
                    char tailOp = opsTail->data;

                    if(valsHead == NULL || ((tailOp == '+' || tailOp == '-' || tailOp == '*' || tailOp == '/' || tailOp == '^') && valsHead->next == NULL)) {
                        return NAN;
                    }

                    else if(tailOp == 's' || tailOp == 'c' || tailOp == 't' || tailOp == 'q' || tailOp == 'l' || tailOp == 'e') {
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
            }
            
            if(input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '^' ||
               input[i] == 's' || input[i] == 'c' || input[i] == 't' || input[i] == 'q' || input[i] == 'l' || input[i] == 'e') {
                // What does this part do?
                if(input[i] != '+' && input[i] != '-') {
                    lastIsDigit = false;
                }

                opsHead->pushToTail(&opsHead, &opsTail, input[i]);
            }
               
            else return NAN;
        }
    }

    //calculate remaining operations
    while(opsHead != NULL) {
        char tailOp = opsTail->data;

        if(valsHead == NULL || ((tailOp == '+' || tailOp == '-' || tailOp == '*' || tailOp == '/' || tailOp == '^') && valsHead->next == NULL)) {
            return NAN;
        }

        else if(tailOp == 's' || tailOp == 'c' || tailOp == 't' || tailOp == 'q' || tailOp == 'l' || tailOp == 'e') {
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

//FUNCTION MAIN MENU
int mainMenu() {
    int menu;
    cout << "=================================" << endl;
    cout << "\t    MAIN MENU\t\t" << endl;
    cout << "=================================" << endl << endl;
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
            cout << "====================================================================" << endl;
            cout << "\t\t\tSCIENTIFIC CALCULATOR\t" << endl;
            cout << "====================================================================" << endl;
            cout << "Available operators:" << endl;
            cout << "Basic operators [+, -, *, /, ^, (, )]" << endl;
            cout << "Advance operators [sin, cos, tan, sqrt, log, ln]" << endl;
            cout << "Please enter your expression(s)! (type DONE to return to main menu)" << endl << endl << endl;

            string input;

            cout << "Enter an expression:" << endl;
            getline(cin, input);
            while(input.compare("DONE") != 0) {
                static int error = 0;
                float result = inputResult(input, valsHead, valsTail, opsHead, opsTail);

                if(isnan(result)) {
                    if(error == 0) error++;
                    eraseLines(3);
                    cout << "Please enter an appropriate expression" << endl;
                }
                else {
                    if(error > 0) {
                        eraseWarning(input);
                        error = 0;
                    }
                    cout << "= " << result << endl << endl;
                    history->enqueue(history, input, result);
                }
                
                cout << "Enter an expression:" << endl;
                getline(cin, input);
            }
        }

        //HISTORY MENU
        else if(menu == 2) {
            cout << "=====================================" << endl;
            cout << "\tCALCULATOR HISTORY\t" << endl;
            cout << "=====================================" << endl;
            cout << "1. Show history list" << endl;
            cout << "2. Pick a history to delete" << endl;
            cout << "3. Delete whole history" << endl;
            cout << "0. Back to main menu" << endl << endl;

            int option;

            cout << "Enter your option: ";
            cin >> option;
            while(option != 0) {
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

                cout << "Enter your option: ";
                cin >> option;
            }
        }

        system("cls"); //clears current menu before moving to another menu

        menu = mainMenu();
    }

    //exits program when user enters 0
    exit(0);

    return 0;
}
