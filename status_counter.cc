#include "status_counter.h"

StatusCounter::StatusCounter() {}

StatusCounter& StatusCounter::get_instance() {
    static StatusCounter instance;
    return instance;
}
