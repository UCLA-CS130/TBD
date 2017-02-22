#ifndef STATUS_COUNTER_H
#define STATUS_COUNTER_H

#include <string>
#include <map>
#include <unordered_map>

class StatusCounter {
public:
    static StatusCounter& get_instance();
    StatusCounter(StatusCounter const&) = delete;
    void operator=(StatusCounter const&) = delete;

    int request_count_ = 0;
    std::unordered_map<std::string, std::string> handler_name_map_;
    std::unordered_map<std::string, std::map<int, int>> status_code_map_;
private:
    StatusCounter();
};

#endif
