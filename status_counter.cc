HandlerInfo::HandlerInfo(std::string handler_name)
    : handler_name_(handler_name) {}

void HandlerInfo::increment_count(int status_code) {
    if (status_code_map_.find(status_code) == status_code_map_.end()) {
        status_code_map_[status_code] = 1;
    } else {
        status_code_map_[status_code] += 1;
    }
}
