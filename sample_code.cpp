#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <set>
using namespace std;

#define OPERATORS "+-*/"

typedef map<string, int> Memory;
typedef string Name;

class Expr {
public:
    static Expr* parse(istream& in);
    virtual ~Expr() {}
    virtual int eval(Memory mem) = 0;
    virtual set<Name> read_set() = 0;
    virtual string str() = 0;
};

class BinOp : public Expr {
public:
    BinOp(char rator_, Expr* left_, Expr* right_) : rator(rator_), left(left_), right(right_) {}
    ~BinOp() {
        delete left;
        delete right;
    }
    virtual int eval(Memory mem) {
        switch (rator) {
            case '+': return left->eval(mem) + right->eval(mem);
            case '-': return left->eval(mem) - right->eval(mem);
            case '*': return left->eval(mem) * right->eval(mem);
            case '/': return left->eval(mem) / right->eval(mem);
            default: throw runtime_error("invalid operator encountered");
        }
    }
    set<Name> read_set() {
        set<Name> result = left->read_set();
        for (auto n : right->read_set())
            result.insert(n);
        return result;
    }

    string str() {
        return left->str() + " " + rator + " " + right->str();
    }
private:
    char rator;
    Expr* left;
    Expr* right;
};

class Variable : public Expr {
public:
    static Variable* parse(istream& in) {
        string s;
        in >> ws;
        while (isalpha(in.peek())) {
            char c;
            in.get(c);
            s.push_back(c);
        }
        return new Variable(s);
    }

    Name get_name() { return name; }

    virtual int eval(Memory mem) {
        return mem[name];
    }

    set<Name> read_set() {
        set<Name> result;
        result.insert(name);
        return result;
    }

    string str() {
        return name;
    }
private:
    Variable(Name name_) : name(name_) {}
    Name name;
};

class Number : public Expr {
public:
    static Number* parse(istream& in) {
        int n;
        in >> ws >> n;
        return new Number(n);
    }
    virtual int eval(Memory) {
        return value;
    }

    set<Name> read_set() {
        return set<Name>();
    }

    string str() {
        return to_string(value);
    }
private:
    Number(int value_) : value(value_) {}
    int value;
};

class WSet {
public:
    WSet() : empty(true) {}
    WSet(Name name_) : empty(false), name(name_) {}
    bool operator==(const WSet& other) {
        return (!empty && !other.empty) && (name == other.name);
    }
    bool in(const set<Name>& names) {
        return !empty && names.count(name) != 0;
    }
    bool empty;
    Name name;
};

class Command {
public:
    static Command* parse(istream& in);
    virtual ~Command() {}
    virtual void exec(Memory& mem, istream& input) = 0;
    virtual set<Name> read_set() = 0;
    virtual WSet write_set() = 0;
    virtual string str() = 0;
    bool must_be_after(Command* cmd);
};

class Input : public Command {
public:
    static Command* parse(istream& in);
    Input(Variable* var_) : var(var_) {}
    ~Input() {
        delete var;
    }
    virtual void exec(Memory& mem, istream& input) {
        int x;
        input >> x;
        mem[var->get_name()] = x;
    }

    set<Name> read_set() {
        return set<Name>();
    }

    WSet write_set() {
        return WSet(var->get_name());
    }

    string str() {
        return "? " + var->str();
    }
private:
    Variable* var;
};

class Output : public Command {
public:
    static Command* parse(istream& in);
    Output(Expr* expr_) : expr(expr_) {}
    ~Output() {
        delete expr;
    }
    virtual void exec(Memory& mem, istream& input) {
        cout << expr->eval(mem) << endl;
    }

    string str() {
        return "! " + expr->str();
    }

    set<Name> read_set() {
        return expr->read_set();
    }

    WSet write_set() {
        return WSet();
    }
private:
    Expr* expr;
};

class Assignment : public Command {
public:
    static Command* parse(istream& in);
    Assignment(Variable* left_, Expr* right_) : left(left_), right(right_) {}
    ~Assignment() {
        delete left;
        delete right;
    }
    virtual void exec(Memory& mem, istream& input) {
        mem[left->get_name()] = right->eval(mem);
    }

    set<Name> read_set() {
        return right->read_set();
    }

    WSet write_set() {
        return WSet(left->get_name());
    }

    string str() {
        return left->str() + " = " + right->str();
    }
private:
    Variable* left;
    Expr* right;
};

class Program {
public:
    Program(istream& in) {
        Command* cmd = Command::parse(in);
        while (cmd) {
            commands.push_back(cmd);
            cmd = Command::parse(in);
        }
    }
    ~Program() {
        for (auto cmd : commands)
            delete cmd;
    }
    void exec(Memory& mem, istream& input) {
        int i = 1;
        for (auto cmd : commands)
            cmd->exec(mem, input);
    }

    string str() {
        ostringstream oss;
        for (auto cmd : commands) {
            oss << cmd->str() << endl;
        }
        oss << endl;
        return oss.str();
    }
    int calc_min_steps() {
        ofstream dump("dump.txt");
        vector<int> earliest_step(commands.size(), 0);
        int min_step = 0;
        for (int i = 0; i < commands.size(); i++) {
            for (int j = 0; j < i; j++) {
                if (commands[i]->must_be_after(commands[j]))
                    if (earliest_step[i]  < earliest_step[j] + 1)
                        earliest_step[i]  = earliest_step[j] + 1;
            }
            if (min_step < earliest_step[i])
                min_step = earliest_step[i];
            dump << earliest_step[i] << '\t' << commands[i]->str() << endl;
        }
        dump.close();
        return min_step + 1;
    }
private:
    vector<Command*> commands;
};

Expr* Expr::parse(istream& in) {
    in >> ws;
    char lookahead = in.peek();
    Expr* left;
    if (isdigit(lookahead))
        left = Number::parse(in);
    else if (isalpha(lookahead))
        left = Variable::parse(in);
    else
        throw runtime_error("invalid character in expression");
    in >> ws;
    if (strchr(OPERATORS, in.peek())) {
        char rator;
        in.get(rator);
        Expr* right = parse(in);
        return new BinOp(rator, left, right);
    } else
        return left;
}

Command* Command::parse(istream& in) {
    in >> ws;
    char lookahead = in.peek();
    if (lookahead == '$')
        return NULL;        // end of program
    if (lookahead == '?')
        return Input::parse(in);
    if (lookahead == '!')
        return Output::parse(in);
    if (isalpha(lookahead))
        return Assignment::parse(in);
    throw runtime_error("invalid character starting a command");
}

Command* Input::parse(istream& in) {
    in >> ws;
    char ch;
    in.get(ch);
    if (ch != '?')
        throw runtime_error("invalid character starting an input command");
    return new Input(Variable::parse(in));
}

Command* Output::parse(istream& in) {
    in >> ws;
    char ch;
    in.get(ch);
    if (ch != '!')
        throw runtime_error("invalid character starting an output command");
    return new Output(Expr::parse(in));
}

Command* Assignment::parse(istream& in) {
    Variable* var = Variable::parse(in);
    in >> ws;
    char ch;
    in.get(ch);
    if (ch != '=')
        throw runtime_error("= expected in assignment command");
    return new Assignment(var, Expr::parse(in));
}

bool Command::must_be_after(Command* cmd) {
    return
            (dynamic_cast<Input*>(this) && dynamic_cast<Input*>(cmd)) ||
            this->write_set() == cmd->write_set() ||
            cmd->write_set().in(this->read_set()) ||
            this->write_set().in(cmd->read_set());
}

int main() {
//    ifstream input("input.zab");
    istream& input = cin;
    Program p(input);
    input.get();    // consume '$'

    Memory mem;
    p.exec(mem, input);

    cout << "Minimum Steps: " << p.calc_min_steps() << endl;
}