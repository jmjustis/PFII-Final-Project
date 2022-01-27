#include "Expression.h"

using namespace std;

Expression::Expression(){
    original = "";
    tokenized = {};
    postfixed = {};
    type = Bad;
    valid = false;
}
    
Expression::Expression(const string& s){
    this->set(s);
}
    
void Expression::set(const string& s){
    original = s;
    string chopped = original;
    string delimiter = " ";
    char* start = &chopped[0];
    
    //insert space imbetween tokens
    
    for (int i = 0; chopped[i] != '\0'; i++){
        char temp = chopped[i];
        if (temp == '+' or  temp == '-' or temp == '*' or temp == '/' or temp == '=' or temp == '(' or temp == ')'){
            chopped.insert(i, " ");
            chopped.insert(i + 2, " ");
            i++;
        }
    }
    
    chopped.push_back(delimiter[0]);
    
    //tokenize based on space delimeter
    
    while(!chopped.empty()){
    
    //erase spaces at start of string
    
        int eraseindex = 0;
        while (chopped[eraseindex] == delimiter[0]){
            eraseindex ++;
        }
    
        chopped.erase(0, eraseindex);
    
    //set the characters needed for token into string
    
        string tokstring;
    
        int index = 0; 
        tokstring = chopped[index];
    
        while (chopped[index] != delimiter[0]){
            tokstring.append(&chopped[index], index);
            index++;
        }
    
        for(int i = 0; tokstring[i] != '\0'; i++){
            if (tokstring[i] == delimiter[0]){
                tokstring.erase(i);
            }
        }
    
    //tokenize the new string
    
        Token token(tokstring);
        tokenized.push_back(token);
    
    //erase the token already used
    
        chopped.erase(0, index + 1);
        
        
        //cout << chopped << endl;
    }
    
    //Check for type of Expression
    
    bool integer = true;
    
    for(int i = 0; i < tokenized.size(); i++){
        if (tokenized[i].get_type() == Identifier){
            integer = false;
        }
    }
    if (integer != false){
        type = Arithmetic;
    }
    
    //postfix
    if (this->isValid()){
        valid = true;
        this->postfix();
        this->prefix();
        this->parenthesize();
    }else{
        valid = false;
    }
    
}

void Expression::postfix(){
    vector<Token> ops;
    //int opsindex = 0;
    
    //Cycles through tokens and pushes identifiers and integers onto postfix
    
    for(int i = 0; i < tokenized.size(); i++){
        if (tokenized[i].get_type() == Identifier or tokenized[i].get_type() == Integer){
            postfixed.push_back(tokenized[i]);
            
        //Open brace pushed onto operator stack
        
        } else if (tokenized[i].get_type() == OpenBrace){
            ops.push_back(tokenized[i]);
            
        //Close brace pushes operators onto stack until open brace is reached, then both popped
            
        } else if (tokenized[i].get_type() == CloseBrace){
            for(int j = ops.size() - 1; ops[j].get_type() != OpenBrace; j--){
                postfixed.push_back(ops[j]);
                ops.pop_back();
                //opsindex--;
            }
            ops.pop_back();
            
        //Operators pushed onto stack
            
        } else if (tokenized[i].get_type() == Operators){
            while (!ops.empty() && ops.back().get_priority() >= tokenized[i].get_priority()){
                postfixed.push_back(ops.back());
                ops.pop_back();
                //opsindex--;
            }
            ops.push_back(tokenized[i]);
            //opsindex++;
        }
    }
    
    while (!ops.empty()){
        postfixed.push_back(ops.back());
        ops.pop_back();
        //opsindex--;
    }
    
    
}

void Expression::prefix(){
    vector<Token> reversed = reverse(tokenized);
    
    vector<Token> ops;
    //int opsindex = 0;
    
    //Cycles through tokens and pushes identifiers and integers onto postfix
    
    for(int i = 0; i < reversed.size(); i++){
        if (reversed[i].get_type() == Identifier or reversed[i].get_type() == Integer){
            prefixed.push_back(reversed[i]);
            
        //Open brace pushed onto operator stack
        
        } else if (reversed[i].get_type() == OpenBrace){
            ops.push_back(reversed[i]);
            
        //Close brace pushes operators onto stack until open brace is reached, then both popped
            
        } else if (reversed[i].get_type() == CloseBrace){
            for(int j = ops.size() - 1; ops[j].get_type() != OpenBrace && j > -1; j--){
                prefixed.push_back(ops[j]);
                ops.pop_back();
                //opsindex--;
            }
            ops.pop_back();
            
        //Operators pushed onto stack
            
        } else if (reversed[i].get_type() == Operators){
            while (!ops.empty() && ops.back().get_priority() > reversed[i].get_priority()){
                prefixed.push_back(ops.back());
                ops.pop_back();
                //opsindex--;
            }
            ops.push_back(reversed[i]);
            //opsindex++;
        }
    }
    
    while (!ops.empty()){
        prefixed.push_back(ops.back());
        ops.pop_back();
        //opsindex--;
    }
    
    prefixed = reverse(prefixed);
}

void Expression::parenthesize(){
    Token open;
    Token close;
    open.set("(");
    close.set(")");
    
    vector<Token> ops;
    //vector<Token> paretheses;
    int count = 0;
    int index = 0;
    
    for (int i = 0; i <= tokenized.size(); i++){
        //if open brace, check to see if immediatly closed, if yes move on, if not close it
        if (tokenized[i].get_type() == OpenBrace){
            if (tokenized[i + 4].get_type() == CloseBrace){
                for (int j = i; j <= i + 4; j++){
                    parenthesized.push_back(tokenized[j]);
                    //i++;
                    index++;
                }
                i = i + 4;
            }else {
                parenthesized.push_back(tokenized[i]);
                count++;
                index++;
            }
        }
        //if operator push to stack
        else if (tokenized[i].get_type() == Integer or tokenized[i].get_type() == Identifier){
            parenthesized.push_back(tokenized[i]);
            index++;
        }
        
        //if close brace, if next token is null, add close braces equal to count
        //if not push the brace and continue
        else if (tokenized[i].get_type() == CloseBrace){
            if (i == tokenized.size()){
                for (int j = 0; j <= count; j++){
                    parenthesized.push_back(tokenized[i]);
                    index++;
                    count--;
                }
                return;
            } else {
                parenthesized.push_back(tokenized[i]);
                index++;
                count--;
            }
        }
        
        //if high priority operator, parethesize operands on both sides
        else if (tokenized[i].get_type() == Operators && tokenized[i].get_priority() > 1){
            parenthesized.push_back(tokenized[i]);
            index++;
            
            if (tokenized[i - 1].get_type() == CloseBrace){
                if (tokenized[i + 1].get_type() == OpenBrace){
                    auto it = parenthesized.insert(parenthesized.begin(), open);
                    count++;
                }
                else {
                    int count2 = 0;
                    for (int j = i - 2; tokenized[j].get_type() != OpenBrace; j--){
                        count2++;
                    }  
                    auto it = parenthesized.insert(parenthesized.end() - (2 + count2), open);
                    count++;
                }
            }
            else if (tokenized[i - 2].get_type() == Operators && tokenized[i - 2].get_priority() == 2){
                if (tokenized[i - 3].get_type() == CloseBrace){
                    auto it = parenthesized.insert(parenthesized.end() - 8, open);
                } else {
                    auto it = parenthesized.insert(parenthesized.end() - 5, open);
                }
            } else {
                auto it = parenthesized.insert(parenthesized.end() - 2, open);
            }
            if (tokenized[i + 1].get_type() != OpenBrace){
                i++;
                parenthesized.push_back(tokenized[i]);
                //i++;
                parenthesized.push_back(close);
                count--;
            }
            else {
                count++;
            }
            //parenthesized.insert(it, open);
        }
        
        //if low priority operator, check if next operator is high priority
        //if yes, parenthesize first operator and add to count for end
        //if no or no next operator, parenthesize both operators 
        else if (tokenized[i].get_type() == Operators && tokenized[i].get_priority() == 1){
            if (tokenized[i + 1].get_type() == OpenBrace){
                parenthesized.push_back(tokenized[i]);
            }
            else if (tokenized[i + 2].get_priority() != 2){
                auto it = parenthesized.insert(parenthesized.begin(), open);
                
                
                parenthesized.push_back(tokenized[i]);
                i++;
                /*
                if (tokenized[i].get_type() == OpenBrace){
                    parenthesized.push_back(tokenized[i]);
                    i++;
                }
                */
                parenthesized.push_back(tokenized[i]);
                //i++;
                parenthesized.push_back(close);
            } else {
                auto it = parenthesized.insert(parenthesized.begin(), open);
                parenthesized.push_back(tokenized[i]);
                count++;
            }
        }
        
    }
    Token temp;
    temp.set(")");
    for (int j = 0; j < count; j++){
        parenthesized.push_back(temp);
        index++;
    }
}
   
void Expression::display(){
    int numtokens = 0;
    vector<Token> reversed = reverse(tokenized);
    
    cout << "original = " + original << endl;
    
    //cout << tokenized.size() << endl;
    
    cout << "tokenized = ";
    for (int i = 0; i < tokenized.size(); i++){
        cout << tokenized[i].get_token() << ";";
        numtokens++;
    }
    cout << endl;
    
    cout << "Valid = " << valid << endl;
    
    cout << "postfix = ";
    for (int i = 0; i < postfixed.size(); i++){
        cout << postfixed[i].get_token() << ";";
    }
    cout << endl;
    
    cout << "prefix = ";
    for (int i = 0; i < prefixed.size(); i++){
        cout << prefixed[i].get_token() << ";";
    }
    cout << endl;
    
    cout << "parenthesized = ";
    for (int i = 0; i < parenthesized.size(); i++){
        cout << parenthesized[i].get_token() << ";";
    }
    cout << endl;
    cout << "number of tokens = " << numtokens << endl;
    cout << "Expression type = " << type << endl;
}

bool Expression::isValid(){
    enum States {expect_operand, expect_operator, done};
    bool eqtrue = false;
    States state = expect_operand;
    int count = 0;
    type = Arithmetic;
    
    for (int i = 0; state != done && i < tokenized.size(); i++){
        switch (state){
            case expect_operand:
                if (tokenized[i].get_type() == OpenBrace){
                    count++;
                }else if (tokenized[i].get_type() == Identifier or tokenized[i].get_type() == Integer){
                    state = expect_operator;
                }else {
                    state = done;
                    return false;
                }
                break;
            case expect_operator:
                if (tokenized[i].get_type() == CloseBrace){
                    count--;
                    if (count < 0){
                        state = done;
                        return false;
                    }
                }else if (tokenized[i].get_type() == EqualSign){
                    eqtrue = true;
                    state = expect_operand;
                }else if (tokenized[i].get_type() == Operators){
                    state = expect_operand;
                }else{
                    state = done;
                    return true;
                }
                break;
            default:
                break;
                
        }
    }
    if (count != 0){
        return false;
    }
    if (state == expect_operand){
        return false;
    }
    if (eqtrue){
        if (tokenized.size() == 3 && tokenized[0].get_type() == Identifier && tokenized[2].get_type() == Integer){
            type = Assignment;
            
            //define ID value
            
        }else{
            return false;
        }
    }else {
        type = Arithmetic;
        return true;
    }
    return true;
}
    
string Expression::get_original() const{
    return original;
    
}
    
vector<Token> Expression::get_tokenized() const{
    return tokenized;
}

vector<Token> Expression::get_postfixed() const{
    return postfixed;
}

vector<Token> Expression::get_prefixed() const{
    return prefixed;
}

vector<Token> Expression::get_parenthesized() const{
    return parenthesized;
}

vector<Token> Expression::reverse(vector<Token> x){
    Token open;
    Token close;
    open.set("(");
    close.set(")");
    
    for (int i = 0; i < x.size(); i++){
        if (x[i].get_type() == CloseBrace){
            x[i] = open;
        }
        else if (x[i].get_type() == OpenBrace){
            x[i] = close;
        }
    }
    
    vector<Token> placeholder = x;
    
    int j = placeholder.size() - 1;
    for (int i = 0;i <= x.size() && j >= 0; i++){
        x[i] = placeholder[j];
        j--;
    }
    return x;
}

Exp_type Expression::get_type() const{
    return type;
}


string Expression::solve_integer(){
    vector <Token> stack;
    int x = 0;
    int y = 0;
    
    for (int i = 0; i < postfixed.size(); i++){
        if (postfixed[i].get_type() == Integer){
            stack.push_back(postfixed[i]);
        } else if (postfixed[i].get_type() == Operators){
            stringstream first(stack.back().get_token());
            first >> y;
            stack.pop_back();
            
            stringstream second(stack.back().get_token());
            second >> x;
            stack.pop_back();
            
            int answer;
            
            if (postfixed[i].get_token() == "+"){
                answer = x + y;
            } else if (postfixed[i].get_token() == "-"){
                answer = x - y;
            } else if (postfixed[i].get_token() == "/"){
                answer = x / y;
            } else if (postfixed[i].get_token() == "*"){
                answer = x * y;
            }
            
            stringstream last;
            last << answer;
            string solved = last.str();
            
            stack.push_back(solved);
        }
    }
    
    return stack[0].get_token();
}

