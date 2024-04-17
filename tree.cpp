
// Task 1: Write a program that creates a tree in memory for arithmetic expressions. Use
// additional material: parser example in translate.cpp (only for demonstration); an example of
// the Tree class and the main function (tree.cpp); and the input/output example. You may use
// the tree.cpp code provided or use your own code as long as it behaves as expected and matches
// the example.

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

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
            Tree *temp2 = new Tree;
            temp2->op="^";
            tok();
            temp2->left= eval_prim();
            temp2->right= temp;
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
            Tree *temp2 = new Tree;
            temp2->op=peek;
            tok();
            temp2->left= eval_powr();
            temp2->right= temp;
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
            Tree *temp2 = new Tree;
            temp2->op=peek;
            tok();
            temp2->left= eval_term();
            temp2->right= temp;
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

        istringstream is(line);
        gis = &is; // reset input stream
        gid = 0; // reset id counter
        tok(); // load peek token

        Tree * tree = eval_expr();

        cout << "```mermaid\ngraph TD\n"
             << "A(\"" << line << "\")\n"
             << "B(\"" << tree->expr() << "\")\n"
             << "A --> B\n"
             << "style A fill:#ded\n"
             << "style B fill:#dde\n"
             << tree->edges() << "```\n---\n";

        delete tree;
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
            cout << "okay trial 1" << endl;
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
     if (op.empty()) \
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
