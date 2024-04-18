
// Task 1: Write a program that creates a tree in memory for arithmetic expressions. Use
// additional material: parser example in translate.cpp (only for demonstration); an example of
// the Tree class and the main function (tree.cpp); and the input/output example. You may use
// the tree.cpp code provided or use your own code as long as it behaves as expected and matches
// the example.

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;


//function to transform integer to double
int doubleToInt(double value) {
    return static_cast<int>(value);
}

istream * gis;
string peek;

string tok() 
{ 
    auto r = peek; peek = ""; 
     
  if (*gis) 
        (*gis) >> peek;  return r; 
}

int gid = 0;

struct Tree
{
    int id;
    string op;
    string val;
    Tree * left = nullptr, * right = nullptr;

    Tree(): id(++gid) {}
    Tree(string v): id(++gid), val(v) {}
    Tree(string o, Tree * a, Tree * b): id(++gid), op(o), left(a), right(b) {}
    Tree(const Tree &) = delete;
    ~Tree() { delete left; delete right; }

    string str() const;
    string edges() const;
    string expr() const;
    double compute () const;
    bool capability (Tree * b) const;
};


Tree * eval_expr();

Tree * eval_prim()
{
    // FIXME/ 
    //KIND OF BASE CASE 
    Tree *temp= new Tree;
    if (peek == "-")
    {
        tok();
        temp->val = "-" + tok(); // Include the negative sign in the value
    }
    if (peek=="(")
    {
        tok(); // This function would return ( but would move peek to the next element after (
        return eval_expr();
    }
    else
    {
          temp->val= peek;
          tok();
    }
    return temp;
}

Tree * eval_powr()
{
    // FIXME
    Tree *temp= eval_prim();
    for (;;) // double check about the infinite loop
    {
        if (peek =="^")
        {
            tok();
            Tree *tempside = eval_prim();
            Tree *temp2 = new Tree;
            temp2->op="^";
            temp2->right= tempside;
            temp2->left= temp;
            temp=temp2;
        }
        else 
        {
            break;
        }
    }
        return temp;
}

Tree * eval_term()
{
    // FIXME
    Tree *temp= eval_powr();
    for (;;)
    {
        
        if (peek=="*" || peek=="/" || peek=="%"){
            string save = tok();
            Tree *tempside = eval_powr();
            Tree *temp2 = new Tree;
            temp2->op=save;
            temp2->right= tempside;
            temp2->left= temp;
            temp=temp2;
        }
        else 
        {
            break;
        }
    }
   
        return temp;
}

Tree * eval_expr()
{
    // FIXME
    Tree *temp= eval_term();
    for (;;)
    {
        if (peek=="+" || peek=="-"){
            string save = tok();
            Tree *tempside = eval_term();
            Tree *temp2 = new Tree;
            temp2->op= save;
            temp2->left= temp;
            temp2->right= tempside;
            temp=temp2;
        }
        else if (peek.empty()) break;
        else if (peek == ")") { tok(); break; }
        else throw "bad operator [" + peek + "]";
    }
    return temp;
}

int main()
{
try
{
    for (string line; getline(cin, line);)
    {
        if ( line.empty() ) continue;
        if ( line[0] == '#' ) continue;
        if ( line == "end" ) break;

        size_t pos = line.find('=');
        if (pos != string::npos)
        {
            // There is an equals sign in the line
            // Extract substrings before and after the equals sign
            string beforeEquals = line.substr(0, pos);
            string afterEquals = line.substr(pos + 1);

            istringstream is(beforeEquals);
            gis = &is; // reset input stream
            gid = 0; // reset id counter
            tok(); // load peek token

            //Create first tree
            Tree * tree1 = eval_expr();
            cout << "```mermaid\ngraph TD\n"
             << "A(\"" << beforeEquals << "\")\n"
             << "B(\"" << tree1->expr() << "\")\n"
             << "A --> B\n"
             << "style A fill:#ded\n"
             << "style B fill:#dde\n"
             << tree1->edges() << "```\n---\n";
            
            // Create second tree
            istringstream is1(afterEquals);
            gis = &is1; // reset input stream
            gid = 0; // reset id counter
            tok(); // load peek token

            Tree * tree2 = eval_expr();
            cout << "```mermaid\ngraph TD\n"
             << "A(\"" << beforeEquals << "\")\n"
             << "B(\"" << tree2->expr() << "\")\n"
             << "A --> B\n"
             << "style A fill:#ded\n"
             << "style B fill:#dde\n"
             << tree2->edges() << "```\n---\n";

            bool caparesult = tree1 -> capability(tree2);
            cout << "Now capability " << endl;
            cout << "The expression " << line << "is " << caparesult << endl;
            delete tree1;
            delete tree2;
        }
        else
        {
            // There is no equals sign in the line
            istringstream is(line);
            gis = &is; // reset input stream
            gid = 0; // reset id counter
            tok(); // load peek token

            //Create first tree
            Tree * tree1 = eval_expr();
            cout << "```mermaid\ngraph TD\n"
             << "A(\"" << line << "\")\n"
             << "B(\"" << tree1->expr() << "\")\n"
             << "A --> B\n"
             << "style A fill:#ded\n"
             << "style B fill:#dde\n"
             << tree1->edges() << "```\n---\n";

            cout << "Now compute the value " << endl;
            cout << "The expression " << line << " is " << tree1->compute() << endl; 
            delete tree1;
        }
    }
}
    catch (string s) { cout << "Error: " << s << '\n'; }
    catch (...) { cout << "exception\n"; }
}


string Tree::str() const
{
    if ( op.empty() ) return to_string(id) + "[" + val + "]";
    return to_string(id) + "((\"" + op + "\"))";
}

string Tree::edges() const
{
    string result;
    // FIXME
        if(left!=nullptr || right!=nullptr) //if left is not equal to nullptr then by def right should not be either and vice versa, otherwise the tree does not get printed
        {
            if (!op.empty())
            {
                if (!(left->op.empty()))
                {
                    result= result + to_string(id) + "((\"" + op + "\")) --> " + to_string(left->id) + "((\"" + left->op + "\"))" + "\n";
                }
                if (!(right->op.empty()))
                {
                    result= result + to_string(id) + "((\"" + op + "\")) --> " + to_string(right->id) + "((\"" + right->op + "\"))" + "\n";
                }
                if(!(left->val.empty()))
                {
                    result= result + to_string(id) + "((\"" + op + "\")) --> " + to_string(left->id) + "[\"" + left->val + "\"]"+ '\n';
                }
                if(!(right->val.empty()))
                {
                    result= result + to_string(id) + "((\"" + op + "\")) --> " + to_string(right->id) + "[\"" + right->val + "\"]"+ '\n';
                }
                if (!(left->op.empty()))
                {
                    result+= left->edges();
                }
                 if (!(right->op.empty()))
                {
                    result+= right->edges();
                }
            }
        }
    return result;
}


string Tree::expr() const
{
    // FIXME
     if (op.empty()) 
    {
        return val; // Return the value if it's a leaf
    } 
    else {
        string left_expr;
        if (left != nullptr) {left_expr= left->expr();} 
        else {left_expr="";}
        string right_expr ;
        if (right != nullptr) {right_expr= right->expr();} 
        else {right_expr="";}
        return left_expr + " " + op + " " + right_expr;
    }
}

double Tree::compute () const 
{
    double result = 0;

    if (op.empty())
    {
        return stod(val);
    }
    else
    {
    //assuming that the expression will be balanced 
        if (op == "+") 
        {
            result = result + left->compute() + right->compute();
            return result;
        } 
        else if (op == "-") 
        {
            result = result + left->compute() - right->compute();
            return result;
        } 
        else if (op == "*") 
        {
            result = result + left->compute() * right->compute();
            return result;
        } 
        else if (op == "/") 
        {
            double save = right->compute();
            if (save == 0) 
            {
                    // Handle division by zero error
                cout << "You are dividing by zero" << endl;
                throw ;
            } 
            else 
            {
                result = result + left->compute() / save;
                return result;
            }
        } 
        else if (op == "%")
        {
            double save1 = right->compute();
            double save2= left -> compute();
            int rightval = doubleToInt(save1);
            int leftval = doubleToInt(save2);

            if (rightval == 0) 
            {
                // Handle division by zero error
                cout << "You are dividing by zero" << endl;
                throw ;
            } 
            else 
            {
                result = result + leftval % rightval;
                return result;
            }
        } 
        else if (op == "^") 
        {
            result = pow(left->compute(), right->compute()); // Using the pow function from cmath for exponentiation
            return result;
        } 
        else 
        {
            // Handle invalid operator
            cout << "Invalid operator" << endl;
            return -100000000;
        }
    }

}

bool Tree::capability (Tree * b) const
{
    double first= compute();
    double second = b->compute ();
  if (first == second){
    cout << first << "==" << second << endl;
    return true;
  }
  else {
    cout << " The results of trees are not equal "<< first << "!=" << second << endl;
    return false;
  }
  
}
