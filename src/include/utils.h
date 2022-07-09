#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

std::string extract_token(const std::vector<char>::const_iterator& lexeme_begin,
                          const std::vector<char>::const_iterator& forward)
{
    std::string token(lexeme_begin, forward);
    return token;
}

int read_file(std::vector<char>& buffer, std::string filename)
{
    std::ifstream instream(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (instream) {
        std::size_t size = instream.tellg();
        instream.seekg(0, std::ios::beg);

        buffer.resize(size);
        instream.read(buffer.data(), size);
        if (!instream) {
            std::cerr << "failed to read file: " + filename + "\n";
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    else {
        std::cerr << "failed to open file: " + filename + "\n";
        return EXIT_FAILURE;
    }
}

std::vector<double> tokenize_buffer(const std::vector<char>& buffer)
{
    std::vector<char>::const_iterator lexeme_begin = begin(buffer);
    std::vector<char>::const_iterator forward = begin(buffer);
    std::vector<double> tokens;

    if (begin(buffer) != end(buffer)) {
        while (forward != end(buffer)) {
            if (*forward == ' ' || *forward == '\n' || *forward == '\0' || *forward == EOF) {
                tokens.push_back(std::stod(extract_token(lexeme_begin, forward)));
                lexeme_begin = forward + 1;
            }
            ++forward;
        }
        if (forward != lexeme_begin)
            tokens.push_back(std::stod(extract_token(lexeme_begin, forward)));
    }
    return tokens;
}

#endif // !UTILS_H