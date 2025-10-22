#include <cstdlib>
#include <filesystem>
#include <string>

namespace Im
{
    static const std::filesystem::path CONST_APP_DIR =
        std::filesystem::path(std::getenv("USERPROFILE")) / "Downloads" / "Limit_order_book";
}
