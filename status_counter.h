#ifndef STATUS_COUNTER_H
#define STATUS_COUNTER_H

#include <string>
#include <map>

class StatusCounter {
public:

private:
};

class HandlerInfo {
public:
    HandlerInfo(std::string handler_name);
    void increment_count(int status_code);

private:
    std::string handler_name_;
    std::map<int, int> status_code_map_;
};

#endif
