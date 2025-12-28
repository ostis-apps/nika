#include "MessageClassificationHelper.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <curl/curl.h>

using namespace messageClassificationModule;

// MessageClassificationHelper::MessageClassificationHelper() {}

std::string MessageClassificationHelper::unicode_escape_to_utf8(const std::string& input) {
    std::ostringstream result;
    
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\\' && i + 1 < input.size() && input[i + 1] == 'u') {
            if (i + 6 <= input.size()) {
                std::string hex = input.substr(i + 2, 4);
                char32_t codepoint = static_cast<char32_t>(std::stoul(hex, nullptr, 16));

                if (codepoint <= 0x7F) {
                    result << static_cast<char>(codepoint); // 1-byte UTF-8
                } else if (codepoint <= 0x7FF) {
                    result << static_cast<char>((codepoint >> 6) | 0xC0)
                           << static_cast<char>((codepoint & 0x3F) | 0x80); // 2-byte UTF-8
                } else if (codepoint <= 0xFFFF) {
                    result << static_cast<char>((codepoint >> 12) | 0xE0)
                           << static_cast<char>(((codepoint >> 6) & 0x3F) | 0x80)
                           << static_cast<char>((codepoint & 0x3F) | 0x80); // 3-byte UTF-8
                } else if (codepoint <= 0x10FFFF) {
                    result << static_cast<char>((codepoint >> 18) | 0xF0)
                           << static_cast<char>(((codepoint >> 12) & 0x3F) | 0x80)
                           << static_cast<char>(((codepoint >> 6) & 0x3F) | 0x80)
                           << static_cast<char>((codepoint & 0x3F) | 0x80); // 4-byte UTF-8
                }
                i += 5;
            }
        } else {
            result << input[i];
        }
    }
    
    return result.str();
}
