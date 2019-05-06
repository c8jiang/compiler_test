#pragma once
#ifndef _SYMBOLS_HPP
#define _SYMBOLS_HPP

#include <unordered_map>

#include "lexer.hpp"

class Id;

namespace symbols
{
using namespace lexer;

// class Env
class Env
{
public:
    Env();
    Env(Env *n);
    void put(Token w, Id i);
    Id get(Token w);

protected:
    Env *prev;

private:
    std::unordered_map<Token, Id> table;
};

// class Type
class Type : public Word
{
public:
    Type(std::string s, int tag, int w);
    static bool numeric(Type p);
    static Type max(Type p1, Type p2);

    int width;
};

Type::Type(std::string s, int tag, int w)
    : Word(s, tag), width(w)
{
}

class TypeNull : public Type
{
public:
    TypeNull() : Type("null", -1, -1) {}
};

static Type int_("int", Tag::BASIC, 4);
static Type float_("float", Tag::BASIC, 8);
static Type char_("char", Tag::BASIC, 1);
static Type bool_("bool", Tag::BASIC, 1);

// class Array
class Array : public Type
{
public:
    Array(int sz, Type p);
    std::string to_string();

    Type of;
    int size;
};

Array::Array(int sz, Type p)
    : Type("[]", Tag::INDEX, sz * p.width), size(sz), of(p)
{
}

std::string Array::to_string()
{
    return "[" + std::to_string(size) + "] " + of.to_string();
}

} // namespace symbols

#endif