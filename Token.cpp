#include "Token.h"

using namespace std;

Token::Token(){
    type = INVALID;
    token = "";
    priority = -1;
}

Token::Token(string s){
    set(s);
}
    
void Token::set(string s){
   token = s;
   bool digit;
   
    if (isdigit(s[0]) == 0){
        digit = false;
    } else {
        digit = true;
    }
   
    if (token == "+" or token == "-"){
       type = Operators;
       priority = 1;
    } else if (token == "*" or token == "/"){
       type = Operators;
       priority = 2;
    } else if (token == "="){
       type = EqualSign;
       priority = -1;
    } else if (token == "("){
       type = OpenBrace;
       priority = 0;
    } else if (token == ")"){
       type = CloseBrace;
       priority = -1;
    } else if (digit == true){
        if (token[0] == 0){
           type = INVALID;
           priority = -1;
        } else {
           type = Integer;
           priority = -1;
        }
    } else {
       type = Identifier;
       priority = -1;
    }
   
}
    
int Token::value() const {
    if (type == Integer){
        stringstream degree(token);
        int x = 0;
        degree >> x;
        return x;
    } else if (type == Identifier){
        return -1;
    } else {
        return -2;
    }
}

void Token::display() const{
    cout << "type = " << type << endl;
    cout << "token = " << token << endl;
    cout << "priority = " << priority << endl;
}
    
Token_type Token::get_type() const{
    return type;
}
    
string Token::get_token() const{
    return token;
}

int Token::get_priority() const{
    return priority;
}
