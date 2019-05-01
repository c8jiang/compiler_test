#ifndef _LEXER_HPP
#define _LEXER_HPP

#include <string>
#include <unordered_map>
#include <fstream>

class Token
{
public:
    Token(int t) : tag(t) {}

    int tag;
};

class Tag
{
public:
    static const int NUM = 256;
    static const int ID = 257;
    static const int TRUE = 258;
    static const int FALSE = 259;
};

class Num : public Token
{
public:
    Num(int v) : Token(Tag::NUM), value(v) {}

    int value;
};

class Word : public Token
{
public:
    Word(int t, std::string s) : Token(t), lexeme(s) {}

    std::string lexeme;
};

class lexer
{
public:
    lexer(const char* file);

    void reserve(Word t);
    Token scan();

    int line;
    char peek;
    std::unordered_map<std::string, Word> words;
    static std::fstream fs;
};

#include "lexer_impl.hpp"

#endif