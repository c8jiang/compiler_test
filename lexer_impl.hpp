#include "basic.hpp"
#include "lexer.hpp"

std::fstream lexer::fs;

lexer::lexer(const char* file) : line(1), peek(' ')
{
    fs.open(file, std::ios_base::in);
    if (!fs.is_open())
        throw std::exception();

    reserve(Word(Tag::TRUE, "true"));
    reserve(Word(Tag::FALSE, "false"));
}

void lexer::reserve(Word t) { words.insert(t.lexeme, t); }

Token lexer::scan()
{
    for (;; peek = fs.get())
    {
        if (peek == ' ')
            continue;
        else if (peek == '\t')
            continue;
        else if (peek == '\n')
            line++;
        else
            break;
    }

    if (is_digit(peek))
    {
        int v = 0;
        do
        {
            v = 10 * v + std::stoul(peek);
            peek = fs.get();
        } while (is_digit(peek));
        return Num(v);
    }

    if (is_letter(peek))
    {
        std::string s;
        do
        {
            s.push_back(peek);
            peek = fs.get();
        } while (is_letter_or_digit(peek));

        auto w = words.find(s);
        if (w != words.end())
            return w->second;

        Word nw(Tag::ID, s);
        words.insert(s, nw);
        return nw;
    }
    Token t(peek);
    peek = ' ';
    return t;
}