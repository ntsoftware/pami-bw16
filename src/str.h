#pragma once

#include <stddef.h>

class IPAddress;

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
    bool parse_str(char *dest, size_t n);
    bool parse_int(int &out);
    bool parse_ip(IPAddress &out);
private:
    const char *ptr;
    size_t len;
};
