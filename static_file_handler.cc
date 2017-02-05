#include "static_file_handler.h"

StaticFileHandler::StaticFileHandler(HttpRequest* http_request) {
    path_ = boost::filesystem::path(http_request->request_path_);
}

StaticFileHandler::~StaticFileHandler() {}

std::string StaticFileHandler::build_response() {
    std::string response = build_status_line(200);
    std::string file_content = read_file();
    response += build_header("Content-Type", "text/plain");
    response += "\r\n" + file_content;
    return response;
}

/*std::string StaticFileHandler::get_mime_type() {
    //if (path_.extension)
}*/

std::string StaticFileHandler::read_file() {
    boost::filesystem::ifstream fs(path_);
    std::string line;
    std::string file_content;
    if (fs.is_open()) {
        while (std::getline(fs, line)) {
            file_content += line + '\n';
        }
        fs.close();
        return file_content;
    } else {
        return "";
    }
}
