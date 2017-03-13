#include "compressor.h"

// returns gzip compressed input string
std::string Compressor::compress(const std::string& original) {
    std::stringstream compressed;
    std::stringstream uncompressed(original);
    boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
    out.push(boost::iostreams::gzip_compressor(boost::iostreams::gzip_params(boost::iostreams::gzip::best_compression)));
    out.push(uncompressed);
    boost::iostreams::copy(out, compressed);
    return compressed.str();
}