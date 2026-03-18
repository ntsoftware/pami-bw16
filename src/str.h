#pragma once

#include <stddef.h>

// suppress strncpy macro definition which conflicts with str::strncpy()
#undef strncpy

class str {
public:
    str(const char *ptr, size_t len);
    void print() const;
    bool is_empty() const;
    str& ltrim();
    str& rtrim();
    int pop();
    int peek() const;
    str split(char delim);
    str split(const char *delim);
    bool equals(const char *other) const;
    bool starts_with(const char *prefix) const;
    bool remove_prefix(const char *prefix);
    void strncpy(char *dest, size_t n) const;
private:
    const char *ptr;
    size_t len;
};
