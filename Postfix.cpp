#include "Postfix.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>


// Convert a variable character to an index in the symbol table (A=0, B=1, ..., Z=25)
int Postfix::getIndex(char var) {
    return var - 'A';
}

//we search for index of variable and return the value of the variable
double Postfix::search(char var) {
    int index = getIndex(var);
    return symbolTable[index];
}


//we  fill in the symbolTable with value at the right index and variable
void Postfix::insert(char var, double value) {
    int index = getIndex(var);
    symbolTable[index] = value;
    printSymbolTable();
}


//we print SymbolTable so that we can check that it is filled
void Postfix::printSymbolTable() {
    std::cout << "Symbol Table:" << std::endl;
    for (int i = 0; i < NUM_VARS; ++i) {
        if (symbolTable[i] != 0) {
            std::cout << static_cast<char>('A' + i) << ": " << symbolTable[i] << std::endl;
        }
    }
}


// help determine whether the expression is a variable or not 
bool isVariable(double value) {
    return value >= 'A' && value <= 'Z';
}




double Postfix::differentiate_input(const std::string& expression) {
    // read from input string,so that then we extract tokens
    std::istringstream tokens(expression);
    std::string token;


    //extract tokens separated by whitespaces, assign then to token 
    while (tokens >> token) {
        //push token to stack , if digits in double format
        if (isdigit(token[0]) || (token.size() > 1)) {
            Stack.push(std::stod(token));
        //push token alphabetic character to stack , letters in ascii format
        } 
        else if (isalpha(token[0]) && token.size() == 1) {
            Stack.push(static_cast<double>(token[0]));
        } 
        //if token arithmetic operator (we add exponent and modulus %) perform operation on top 2 elements
        else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^" || token == "%") {
            // added error detection
            if (Stack.size() < 2) {
                throw std::invalid_argument("Insufficient operands for operation");
            }

            // pop the top 2 elements of stack 
            double stack_top2 = Stack.top();
            Stack.pop();
            double stack_top1 = Stack.top();
            Stack.pop();


            // check if stack_top1 is within the ASCII range for uppercase letters if yes convert to   char using static_cast<char>(stack_top1).
            if (isVariable(stack_top1)) {
                stack_top1 = search(static_cast<char>(stack_top1));
            }
            if (isVariable(stack_top2)) {
                stack_top2 = search(static_cast<char>(stack_top2));
            }




            // Compute the result based on the operator  and pushe the result back onto the Stack
            double result = 0;
            if (token == "+") result = stack_top1 + stack_top2;
            else if (token == "-") result = stack_top1 - stack_top2;
            else if (token == "*") result = stack_top1 * stack_top2;
            else if (token == "/") {
                // division by 0 error 
                if (stack_top2 == 0) throw std::invalid_argument("Division by zero");
                result = stack_top1 / stack_top2;
            }
            else if (token == "^") result = std::pow(stack_top1, stack_top2);
            else if (token == "%") result = std::fmod(stack_top1, stack_top2);

            Stack.push(result);
        } 
        // check if we have assignment operator, if yes we make assignment and pop top 2 stack tokens
        else if (token == "=") {
            if (Stack.size() < 2) {
                throw std::invalid_argument("Insufficient operands for assignment");
            }
            double value = Stack.top();
            Stack.pop();
            double var_as_double = Stack.top();
            Stack.pop();
            char var = static_cast<char>(var_as_double);

            // insert into symbolTable
            insert(var, value);
            // push the value to stack
            Stack.push(value);
        } 

        // other case -error
        else {
            throw std::invalid_argument("Invalid token encountered");
        }
    }

    // return the top if stack is not empty
    if (Stack.empty()) {
        return 0;
    } else {
        return Stack.top();
    }
}

int main() {
    // we create object interpreter of class Postfix
    Postfix interpreter;
    std::string input;

    std::cout << "> ";
    // loop; function reads a line of text from the standard input and stores it in the input string
    while (std::getline(std::cin, input)) {
        try {
            // process the input string as a postfix and return the result as double
            double result = interpreter.differentiate_input(input);
            std::cout << result << std::endl;
        } 
        //  catch block to handle any exceptions
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        std::cout << "> ";
    }

    return 0;
}
