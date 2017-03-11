#include <sstream>
#include "static_file_handler.h"
#include "not_found_handler.h"
#include "markdown.h"
#include "compressor.h"

StaticFileHandler::StaticFileHandler() {}

StaticFileHandler::~StaticFileHandler() {}

std::string StaticFileHandler::GetMimeType(const std::string& file_path) {
    size_t pos = file_path.find_last_of(".");
    
    if (pos == std::string::npos || pos == 0) {
        return "text/plain";
    }

    std::string extension = file_path.substr(pos);

    if (extension == ".html") {
        return "text/html";
    } else if (extension == ".txt") {
        return "text/plain";
    } else if (extension == ".jpeg" || extension == ".jpg") {
        return "image/jpeg";
    } else if (extension == ".md") {
        return "text/html";
    } else {
        return "";
    }
}

bool StaticFileHandler::ReadFile(const std::string& file_path, std::string& file_content) {
    boost::filesystem::ifstream fs(file_path);
    std::string line;
    if (fs.is_open()) {
        while (std::getline(fs, line)) {
            file_content += line + '\n';
        }
        fs.close();
        return true;
    } else {
        return false;
    }
}

StaticFileHandler::Status StaticFileHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    uri_prefix_ = uri_prefix;

    if (config.statements_.size() == 0 || config.statements_[0]->tokens_.size() < 2)
        return INTERNAL_ERROR;

    root_path_ = config.statements_[0]->tokens_[1];
    return OK;
}

StaticFileHandler::Status StaticFileHandler::HandleRequest(const Request& request, Response* response) {
    std::string request_uri = request.uri();
    std::string file_path = root_path_ + request_uri.substr(uri_prefix_.size());

    std::string file_content = "";
    if (ReadFile(file_path, file_content)) {
        response->SetStatus(Response::ResponseCode::OK);

        std::string content_type = GetMimeType(file_path);
        if (IsMarkdown(file_path)) {
            file_content = ProcessMarkdown(file_content);
        }
        response->AddHeader("Content-Type", content_type);

        Compressor compressor;
        std::string compressed_file_content = compressor.compress(file_content);
        response->AddHeader("Content-Encoding", "gzip");
        response->SetBody(compressed_file_content);
        return OK;
    } else {
        response->SetStatus(Response::ResponseCode::FILE_NOT_FOUND);
        return FILE_NOT_FOUND;
    }
}

bool StaticFileHandler::IsMarkdown(const std::string& file_path) {
    int length = file_path.size();
    if (length >= 3 && file_path.substr(length - 3) == ".md") {
        return true;
    } else {
        return false;
    }
}

std::string StaticFileHandler::ProcessMarkdown(const std::string& content) {
    // Use cpp-markdown to render md file as html
    markdown::Document doc;
    doc.read(content);
    std::ostringstream oss;
    doc.write(oss);
    return oss.str();
}

std::string StaticFileHandler::GetName() {
    return "StaticFileHandler";
}
