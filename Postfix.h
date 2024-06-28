#include <iostream>
#include <string>
#include <stack>

// number of variables - letters of alphabet, fixed
const int NUM_VARS = 26; 

class Postfix {
    private:
        std::stack<double> Stack;
        double symbolTable[NUM_VARS] = {0};

        int getIndex(char var);
         // Search for the value of a variable in the symbol table
        double search(char var);
        // Insert a value for a variable in the symbol table
        void insert(char var, double value);
        void printSymbolTable();

    public:
        double differentiate_input(const std::string& expression);
};