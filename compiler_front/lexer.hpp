#pragma once
#ifndef _LEXER_HPP
#define _LEXER_HPP

#include <string>
#include <unordered_map>
#include <fstream>
#include <stdexcept>

#include "basic.hpp"

namespace lexer
{

enum Tag
{
    AND = 256,
    BASIC,
    BREAK,
    DO,
    ELSE,
    EQ,
    FALSE,
    GE,
    ID,
    IF,
    INDEX,
    LE,
    MINUS,
    NE,
    NUM,
    OR,
    REAL,
    TEMP,
    TRUE,
    WHILE
};

// class Token
class Token
{
public:
    Token(int t);
    virtual std::string to_string();

    int tag;
};

Token::Token(int t)
    : tag(t)
{
}

std::string Token::to_string()
{
    return std::string(1, (char)tag);
}

// class Num
class Num : public Token
{
public:
    Num(int v);
    std::string to_string();

    int value;
};

Num::Num(int v)
    : Token(Tag::NUM), value(v)
{
}

std::string Num::to_string()
{
    return std::to_string(value);
}

// class Word
class Word : public Token
{
public:
    Word(std::string s, int tag);
    std::string to_string();

    std::string lexeme;
};

Word::Word(std::string s, int tag)
    : Token(tag), lexeme(s)
{
}

std::string Word::to_string()
{
    return lexeme;
}

static Word and_("&&", Tag::AND);
static Word or_("||", Tag::OR);
static Word eq_("==", Tag::EQ);
static Word ne_("!=", Tag::NE);
static Word le_("<=", Tag::LE);
static Word ge_(">=", Tag::GE);
static Word minus_("minus", Tag::MINUS);
static Word true_("true", Tag::TRUE);
static Word false_("false", Tag::FALSE);
static Word temp_("t", Tag::TEMP);

// class Real
class Real : public Token
{
public:
    Real(float v);
    std::string to_string();

    float value;
};

Real::Real(float v)
    : Token(Tag::REAL), value(v)
{
}

std::string Real::to_string()
{
    return std::to_string(value);
}

// class Lexer
class Lexer
{
public:
    Lexer(const char *file);
    void reserve(Word w);
    void readch();
    bool readch(char c);
    Token scan();

    static int line;
    static std::fstream fs;
    char peek;
    std::unordered_map<std::string, Word> words;
};

int Lexer::line = 1;
std::fstream Lexer::fs;

Lexer::Lexer(const char *file)
    : peek(' ')
{
    fs.open(file, std::ios_base::in);
    if (!fs.is_open())
        throw std::invalid_argument("failed to open file");

    reserve(Word("if", Tag::IF));
    reserve(Word("else", Tag::ELSE));
    reserve(Word("while", Tag::WHILE));
    reserve(Word("do", Tag::DO));
    reserve(Word("break", Tag::BREAK));
    reserve(true_);
    reserve(false_);
}

void Lexer::reserve(Word w)
{
    words.insert({w.lexeme, w});
}

void Lexer::readch()
{
    peek = fs.get();
}

bool Lexer::readch(char c)
{
    readch();
    if (peek != c)
        return false;

    peek = ' ';
    return true;
}

Token Lexer::scan()
{
    // skip white space
    for (;; readch())
    {
        if (peek == ' ' || peek == '\t')
            continue;
        else if (peek == '\n')
            line++;
        else
            break;
    }

    // handle composite tokens
    switch (peek)
    {
    case '&':
        if (readch('&'))
            return and_;
        else
            return Token('&');
    case '|':
        if (readch('|'))
            return or_;
        else
            return Token('|');
    case '=':
        if (readch('='))
            return eq_;
        else
            return Token('=');
    case '!':
        if (readch('='))
            return ne_;
        else
            return Token('!');
    case '<':
        if (readch('='))
            return le_;
        else
            return Token('<');
    case '>':
        if (readch('='))
            return ge_;
        else
            return Token('>');
    }

    // handle constant number
    if (is_digit(peek))
    {
        int v = 0;
        do
        {
            v = 10 * v + std::stoul(std::string(1, peek));
            readch();
        } while (is_digit(peek));

        if (peek != '.')
            return Num(v);

        float x = v;
        float d = 10;
        for (;;)
        {
            readch();
            if (!is_digit(peek))
                break;
            x = x + std::stoul(std::string(1, peek)) / d;
            d = d * 10;
        }
        return Real(x);
    }

    // handle identify
    if (is_letter(peek))
    {
        std::string s;
        do
        {
            s.push_back(peek);
            readch();
        } while (is_letter_or_digit(peek));

        // is reserve word
        auto find = words.find(s);
        if (find != words.end())
            return find->second;
        
        Word nw(s, Tag::ID);
        words.insert({s, nw});
        return nw;
    }

    // return remaining characters as token
    Token tok(peek);
    peek = ' ';
    return tok;
}

} // namespace lexer

#endif