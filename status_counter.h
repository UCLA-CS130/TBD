#ifndef STATUS_COUNTER_H
#define STATUS_COUNTER_H

#include <string>
#include <map>
#include <unordered_map>
#include <memory>

class HandlerInfo {
public:
    HandlerInfo(std::string handler_name);
    void increment_count(int status_code);

    std::string handler_name_;
    std::map<int, int> status_code_map_;
};

class StatusCounter {
public:
    static StatusCounter& get_instance();
    StatusCounter(StatusCounter const&) = delete;
    void operator=(StatusCounter const&) = delete;

    int request_count_ = 0;
    std::unordered_map<std::string, std::unique_ptr<HandlerInfo>> handler_info_map_;
private:
    StatusCounter();
};

#endif
