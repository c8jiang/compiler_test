#pragma once
#ifndef _INTER_HPP
#define _INTER_HPP

#include <iostream>

#include "lexer.hpp"
#include "symbols.hpp"

namespace inter
{
using namespace lexer;
using namespace symbols;

// class Node
class Node
{
public:
    Node();
    void error(std::string s);
    int newlabel();
    void emitlabel(int i);
    void emit(std::string s);

    int lexline;
    static int labels;
};

int Node::labels = 0;

Node::Node()
    : lexline(Lexer::line)
{
}

void Node::error(std::string s)
{
    throw std::runtime_error("near line " + std::to_string(lexline) + ": " + s);
}

int Node::newlabel()
{
    return ++labels;
}

void Node::emitlabel(int i)
{
    std::cout << "L" << i << ":";
}

void Node::emit(std::string s)
{
    std::cout << '\t' << s << '\n';
}

// class Expr
class Expr : public Node
{
public:
    Expr(Token tok, Type p);
    virtual Expr gen();
    virtual Expr reduce();
    void jumping(int t, int f);
    void emitjumps(std::string test, int t, int f);
    std::string to_string();

    Token op;
    Type type;
};

Expr::Expr(Token tok, Type p)
    : op(tok), type(p)
{
}

Expr Expr::gen() { return *this; }

Expr Expr::reduce() { return *this; }

void Expr::jumping(int t, int f)
{
    emitjumps(to_string(), t, f);
}

void Expr::emitjumps(std::string test, int t, int f)
{
    if (t != 0 && f != 0)
    {
        emit("if " + test + " goto L" + std::to_string(t));
        emit("goto L" + std::to_string(f));
    }
    else if (t != 0)
        emit("if " + test + " goto L" + std::to_string(t));
    else if (f != 0)
        emit("iffalse " + test + " goto L" + std::to_string(f));
    else
        ;
}

std::string Expr::to_string()
{
    return op.to_string();
}

// class Id
class Id : public Expr
{
public:
    Id(Word id, Type p, int b);

    int offset;
};

Id::Id(Word id, Type p, int b)
    : Expr(id, p), offset(b)
{
}

// class Op
class Op : public Expr
{
public:
    Op(Token tok, Type p);
    Expr reduce();
};

Op::Op(Token tok, Type p)
    : Expr(tok, p)
{
}

Expr Op::reduce()
{
    Expr x = gen();
    auto t(type);
    emit(t.to_string() + " = " + x.to_string());
    return t;
}

// class Arith
class Arith : public Op
{
public:
    Arith(Token tok, Expr x1, Expr x2);
    Expr gen();
    std::string to_string();

    Expr expr1;
    Expr expr2;
};

Arith::Arith(Token tok, Expr x1, Expr x2)
    : Op(tok, TypeNull()), expr1(x1), expr2(x2)
{
    type = Type::max(expr1.type, expr2.type);
    if (type == null)
        error("type error");
}

Expr gen()
{
    return Arith(op, expr1.reduce(), expr2.reduce());
}

std::string to_string()
{
    return expr1.to_string() + " " + op.to_string() + " " + expr2.to_string();
}

} // namespace inter

#endif