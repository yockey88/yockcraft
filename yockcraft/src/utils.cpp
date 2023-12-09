#include "utils.hpp"

#include <fstream>
#include <sstream>

namespace yockcraft {
namespace utils {

    std::string ReadFile(const std::string& file) {
        std::ifstream stream(file);
        std::stringstream buffer;
        buffer << stream.rdbuf();
        return buffer.str();
    }

} // namespace utils
} // namespace yockcraft
