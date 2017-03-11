#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

class Compressor {
public:
    std::string compress(const std::string& original);
};

#endif
