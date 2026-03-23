#pragma once

#include <stddef.h>

#undef strncpy

class IPAddress;

class str {
public:
    str(const char *ptr, size_t len);
    void print() const;
    void println() const;
    bool is_empty() const;
    str& ltrim();
    str& rtrim();
    int pop();
    int peek() const;
    str split(char delim);
    str split(const char *delim);
    bool equals(const char *other) const;
    bool starts_with(const char *prefix) const;
    bool ends_with(const char *suffix) const;
    bool remove_prefix(const char *prefix);
    bool parse_str(char *dest, size_t n);
    bool parse_int(int &out);
    bool parse_ip(IPAddress &out);
    void strncpy(char *dest, size_t n) const;
private:
    const char *ptr;
    size_t len;
};
