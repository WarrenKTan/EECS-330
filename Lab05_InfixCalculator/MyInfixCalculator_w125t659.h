#ifndef __MYINFIXCALCULATOR_H__
#define __MYINFIXCALCULATOR_H__

#include <algorithm>
#include <string>

#include "MyStack_w125t659.h"
#include "MyVector_w125t659.h"

class MyInfixCalculator{

  public:
    
    MyInfixCalculator()
    {

    }

    ~MyInfixCalculator()
    {

    }

    double calculate(const std::string& s)
    {
        MyVector<std::string> infix_tokens;
        MyVector<std::string> postfix_tokens;
        
        tokenize(s, infix_tokens);
        infixToPostfix(infix_tokens, postfix_tokens);
        return calPostfix(postfix_tokens);
    }

  private:

    // returns operator precedance; the smaller the number the higher precedence
    // returns -1 if the operator is invalid
    // does not consider parenthesis
    int operatorPrec(const char c) const
    {
        switch(c)
        {
            case '*':
                return 2;
            case '/':
                return 2;
            case '+':
                return 3;
            case '-':
                return 3;
            default:
                return -1;
        }
    }

    // checks if a character corresponds to a valid parenthesis
    bool isValidParenthesis(const char c) const
    {
        switch(c)
        {
            case '(':
                return true;
            case ')':
                return true;
            default:
                return false;
        }
    }

    // checks if a character corresponds to a valid digit
    bool isDigit(const char c) const
    {
        if(c >= '0' && c <= '9')
            return true;
        return false;
    }

    // computes binary operation given the two operands and the operator in their string form
    double computeBinaryOperation(const std::string& ornd1, const std::string& ornd2, const std::string& opt) const
    {
        double o1 = std::stod(ornd1);
        double o2 = std::stod(ornd2);
        switch(opt[0])
        {
            case '+':
                return o1 + o2;
            case '-':
                return o1 - o2;
            case '*':
                return o1 * o2;
            case '/':
                return o1 / o2;
            default:
                std::cerr << "Error: unrecognized operator: " << opt << std::endl;
                return 0.0;
        }
    }


    // tokenizes an infix string s into a set of tokens (operands or operators)
    void tokenize(const std::string& s, MyVector<std::string>& tokens)
    {
        size_t i = 0;
        while (i < s.length()){
            
            // check numbers
            if(isDigit(s[i]) || s[i] == '.'){
                std::string num;
                
                while(i < s.length() && (isDigit(s[i]) || s[i] == '.')){
                    num += s[i];
                    i++;
                }

                tokens.push_back(num);
            }
            // check for subtraction or negative
            else if (s[i] == '-'){
                //negative number
                if (tokens.empty() || tokens.back() == "(" || (tokens.back().length() == 1 && operatorPrec(tokens.back()[0]) != -1)){
                    std::string num = "-";
                    i++;

                    while(i < s.length() && (isDigit(s[i]) || s[i] == '.')){
                        num += s[i];
                        i++;
                    }
    
                    tokens.push_back(num);
                }
                // subtraction, treat like normal operator
                else{
                    tokens.push_back(std::string(1, s[i]));
                    i++;
                }
            }
            // operators or parenthesis
            else if(isValidParenthesis(s[i]) || operatorPrec(s[i]) != -1){
                tokens.push_back(std::string(1, s[i]));
                i++;
            }
            // invalid input
            else{
                std::cerr << "Error: Invalid character '" << s[i] << "' in input.\n";
                i++;
            }
        }
    }

    // converts a set of infix tokens to a set of postfix tokens
    void infixToPostfix(MyVector<std::string>& infix_tokens, MyVector<std::string>& postfix_tokens)
    {
        MyStack<std::string> stack;

        for(const std::string& token : infix_tokens){

            // digit
            if (isDigit(token[0]) || token.length() > 1 && token[0] == '-'){
                postfix_tokens.push_back(token);
            }
            // open parenthesis
            else if(token == "("){
                stack.push(token);
            }
            // closing parenthesis
            else if(token == ")"){
                // keep popping to list until open parenthesis is found
                while(!(stack.empty()) && stack.top() != "("){
                    postfix_tokens.push_back(stack.top());
                    stack.pop();
                }

                // remove open parenthesis
                stack.pop();
            }
            // operator
            else if(operatorPrec(token[0]) != -1){
                // pop all operators of equal or higher precedence
                while(!(stack.empty()) && stack.top() != "(" && operatorPrec(token[0]) >= operatorPrec(stack.top()[0])){
                    postfix_tokens.push_back(stack.top());
                    stack.pop();
                }
                // push operator to stack
                stack.push(token);
            }
            // invalid token
            else{
                std::cerr << "Error: Invalid token '" << token << "' in input.\n";
            }
        }

        while(!(stack.empty())){
            postfix_tokens.push_back(stack.top());
            stack.pop();
        }
    }

    // calculates the final result from postfix tokens
    double calPostfix(const MyVector<std::string>& postfix_tokens) const
    {
        MyStack<double> stack;

        for(const std::string& token : postfix_tokens){
            // digit
            if(isDigit(token[0]) || token.length() > 1 && token[0] == '-'){
                stack.push(std::stod(token));
            }
            // operator
            else{

                if(stack.size() < 2){
                    std::cerr << "Error: Not enough operands for operator " << token << ".\n";
                    return 0.0;
                }

                // get values from stack
                double right = stack.top();
                stack.pop();
                double left = stack.top();
                stack.pop();

                // apply operator and push to stack
                switch(token[0]){
                    case '+':
                        stack.push(left + right);
                        break;
                    case '-':
                        stack.push(left - right);
                        break;
                    case '*':
                        stack.push(left * right);
                        break;
                    case '/':
                        // zero division
                        if (right == 0)
                        {
                            std::cerr << "Error: Division by zero.\n";
                            return 0.0;
                        }
                        stack.push(left / right);
                        break;
                    default:
                        // unknown operator
                        std::cerr << "Error: Unknown operator '" << token << "'.\n";
                        return 0.0;
                }
            }
        }

        if (stack.size() != 1){
            std::cerr << "Error: Invalid postfix expression.\n";
            return 0.0;
        }

        return stack.top();
    }
};

#endif // __MYINFIXCALCULATOR_H__