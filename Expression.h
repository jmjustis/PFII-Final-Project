#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Token.h"

using namespace std;

enum Exp_type {Assignment, Arithmetic, Bad};

class Expression
{
public:
    Expression();
    Expression(const string& s);
    
    void set(const string& s);
    void display();
    void postfix();
    void prefix();
    void parenthesize();
    
    bool isValid();
    
    string get_original() const;
    
    Exp_type get_type() const;
    
    vector<Token> get_tokenized() const;
    vector<Token> get_postfixed() const;
    vector<Token> get_prefixed() const;
    vector<Token> get_parenthesized() const;
    vector<Token> reverse(vector<Token> x);
    
    string solve_integer();
private:
    string original;
    vector<Token> tokenized;
    vector<Token> postfixed;
    vector<Token> prefixed;
    vector<Token> parenthesized;
    Exp_type type;
    bool valid;
};
#endif