#include "static_file_handler.h"

StaticFileHandler::StaticFileHandler(std::string file_path)
    : file_path_(file_path) {}

StaticFileHandler::~StaticFileHandler() {}

std::string StaticFileHandler::build_response() {
    std::string response = build_status_line(200);
    std::string file_content = "";

    std::string mime_type = get_mime_type();
    bool cannot_open_file = false;
    file_content = read_file(cannot_open_file);
    
    if (cannot_open_file) {
        response = build_status_line(404);
        mime_type = "text/plain";
        file_content = "404 File Not Found\n";
    }

    response += build_header("Content-Type", mime_type);
    response += "\r\n" + file_content;
    return response;
}

std::string StaticFileHandler::get_mime_type() {
    size_t pos = file_path_.find_last_of(".");
    if (pos == std::string::npos) {
        return "text/plain";
    }

    std::string extension = file_path_.substr(pos);

    if (extension == ".html")
        return "text/html";
    else if (extension == ".txt")
        return "text/plain";
    else if (extension == ".jpeg" || extension == ".jpg")
        return "image/jpeg";
    else
        return "";
}

std::string StaticFileHandler::read_file(bool &cannot_open_file) {
    boost::filesystem::ifstream fs(file_path_);
    std::string line;
    std::string file_content;
    if (fs.is_open()) {
        while (std::getline(fs, line)) {
            file_content += line + '\n';
        }
        fs.close();
        return file_content;
    } else {
        cannot_open_file = true;
        return "";
    }
}
