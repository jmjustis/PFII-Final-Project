#include <iostream>
#include <sstream>
#include <ostream>

#include "Expression.h"
#include "Token.h"
using namespace std;

struct Node
    {
        string name;
        string value;
        Node * next;
        Node * prev;
    };

void interactive(){
    string input;
    vector <Expression> list;
    
    Node * head = new Node;
    Node * tail = new Node;
    Node * current;
    
    current = head;
    tail = head;
    
    while (1){
        cout << "input:";
        getline(cin, input);
        string placeholder;
        int index = 0;
        
        for (int i = 0; input[i] != '\0'; i++){
            if (input[i] == ';'){
                for (int j = index; j < i; j++){
                    placeholder.push_back(input[j]);
                }
                Expression temp;
                temp.set(placeholder);
                
                if (temp.get_type() == Assignment){
                    current->name = temp.get_tokenized()[0].get_token();
                    string s = temp.get_tokenized()[2].get_token();
                    
                    
                    current->value = s;
                    current->next = new Node;
                    current = current->next;
                    tail = current;
                }
                
                list.push_back(temp);
                
                index = i + 1;
                placeholder.clear();
                
            }
        }
        
        while(1) {
            cout << "action:";
            getline(cin, input);
            if (input.size() == 1 && (input[0] == 'q' or input[0] == 'Q')){
                return;
            }
            else if (input.size() == 1 && (input[0] == 'c' or input[0] == 'C')){
                break;
            }
            else if (input.size() == 1 && (input[0] == 'd' or input[0] == 'D')){
                for (int i = 0; i < list.size(); i++){
                    list[i].display();
                    cout << endl;
                    
                }
                
                cout << "Identifier list: ";
                
                current = head;
                
                
                while (current != tail){
                    cout << current->name << current->value << ";";
                    current = current->next;
                }
                
                cout << endl;
            }
            else if (input.size() == 1 && (input[0] == 's' or input[0] == 'S')){
                break;
            }
            else if (input.size() == 1 && (input[0] == 'f' or input[0] == 'F')){
                //convert each expression in the sequence of expressions to the equiv- alent fully parenthesized expression.
                
                for (int i = 0; i < list.size(); i++){
                    if (list[i].get_type() != Assignment){
                        cout << "fully parenthesizing ";
                        cout << list[i].get_original();
                        cout << " results: ";
                        vector<Token> temp2 = list[i].get_parenthesized();
                        for (int j = 0; j < temp2.size(); j++){
                            cout << temp2[j].get_token();
                        }
                        cout << endl;
                    } else {
                        cout << "no fully parenthesizing of ";
                        cout << list[i].get_original();
                        cout << " which is not an arithmetic expression, but assignment";
                        cout << endl;
                    }
                }
                cout << endl;
            }
            else if (input.size() == 1 && input[0] == '='){
                //evaluate each expression in the sequence of expressions.
                for (int i = 0; i < list.size(); i++){
                    bool id = false;
                    if (list[i].get_type() == Arithmetic){
                        
                        vector<Token> temp = list[i].get_tokenized();
                        for (int j = 0; j < temp.size(); j++){
                            cout << temp[j].get_token();
                            if (temp[j].get_type() == Identifier){
                                id = true;
                            }
                        }
                        
                        // if id is true, expression contains identifiers that possibly need to be converted
                        string newString;
                        string oldString;
                        int idcount = 0;
                        int replaced = 0;
                        if (id == true){
                            
                            for (int j = 0; j < temp.size(); j++){
                                oldString.push_back(temp[j].get_token()[0]);
                                if (temp[j].get_type() == Identifier){
                                    idcount++;
                                    current = head;
                                    while (current != tail){
                                        if (current->name[0] == temp[j].get_token()[0]){
                                            
                                            newString.push_back(current->value[0]);
                                            replaced++;
                                            
                                        }
                                        current = current->next;
                                    }
                                } else {
                                    
                                    newString.push_back(temp[j].get_token()[0]);
                                }
                                
                            }
                            
                            if (idcount == replaced){
                                Expression temp;
                                temp.set(newString);
                                list[i] = temp;
                            }
                        }
                        
                        cout << " = ";
                        if (idcount == replaced){
                            cout << list[i].solve_integer() << endl;
                        }else {
                            cout << "no result, some variable has undefined value" << endl;
                        }
                        if (id == true && idcount == replaced){
                            Expression temp;
                            temp.set(oldString);
                            list[i] = temp;
                        }
                    }
                    else if (list[i].get_type() == Assignment){
                        vector<Token> temp = list[i].get_tokenized();
                        cout << "cannot evaluate ";
                        cout << list[i].get_original();
                        cout << " which is not an arithmetic expression, but assignment." << endl;
                    }
                    else{
                        cout << "Not solveable" << endl;
                        //fix this with what its supposed to be
                    }
                }
                cout << endl;
            }
            else if (input.size() == 1 && input[0] == '<'){
                //convert each expression in the sequence of expressions to the equiv- alent postfix expression.
                
                for (int i = 0; i < list.size(); i++){
                    if (list[i].get_type() != Assignment){
                        cout << "postfix of ";
                        cout << list[i].get_original();
                        cout << " is: ";
                        vector<Token> temp2 = list[i].get_postfixed();
                        for (int j = 0; j < temp2.size(); j++){
                            cout << temp2[j].get_token();
                            cout << " ";
                        
                        }
                        cout << endl;
                    } else {
                        cout << "no postfix of ";
                        cout << list[i].get_original();
                        cout << " which is not an arithmetic expression, but assignment." << endl;
                    }
                }
                cout << endl;
                
            }
            else if (input.size() == 1 && input[0] == '>'){
                //convert each expression in the sequence of expressions to the equiv- alent prefix expression.
                for (int i = 0; i < list.size(); i++){
                    if (list[i].get_type() != Assignment){
                        cout << "prefix of ";
                        cout << list[i].get_original();
                        cout << " is: ";
                        vector<Token> temp2 = list[i].get_prefixed();
                        for (int j = 0; j < temp2.size(); j++){
                            cout << temp2[j].get_token();
                            cout << " ";
                        }
                    cout << endl;
                    } else {
                        cout << "no prefix of ";
                        cout << list[i].get_original();
                        cout << " which is not an arithmetic expression, but assignment." << endl;
                    }
                }
                cout << endl;
                
            }
            else {
                cout << "Wrong input for the action! Please type one of =, <, >, f(F), q(Q), c(C), s(S)" << endl;
            }
        }
    }
}

int main(){
    cout << "=== expression evaluation program starts ===" << endl;
    interactive();
    cout << "=== expression evaluation program ends ===" << endl;
    return 0;
}
