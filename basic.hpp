#ifndef _BASIC_HPP
#define _BASIC_HPP

bool is_digit(char c) { return '0' <= c && c <= '9'; }

bool is_upper_letter(char c) { return 'A' <= c && c <= 'Z'; }

bool is_lower_letter(char c) { return 'a' <= c && c <= 'z'; }

bool is_letter(char c) { return is_upper_letter(c) || is_lower_letter(c); }

bool is_letter_or_digit(char c) { return is_letter(c) || is_digit(c); }

#endif