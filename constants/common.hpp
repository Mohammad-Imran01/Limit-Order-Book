#include <cstdlib>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;
using SptrLimit = std::shared_ptr<Limit>;
namespace Im
{
    static const fs::path CONST_APP_DIR =
        fs::path(std::getenv("USERPROFILE")) / "Downloads" / "Limit_order_book";

    static const fs::path CONST_APP_ORDER_FILE_PATH =
        fs::path(std::getenv("USERPROFILE")) / "Downloads" / "Limit_order_book" / "initialOrders.txt";

    static const fs::path CONST_APP_INITIAL_ORDER_FILE_PATH =
        fs::path(std::getenv("USERPROFILE")) / "Downloads" / "Limit_order_book" / "Orders.txt";

    static const fs::path CONST_TEST_APP_DIR =
        fs::temp_directory_path() / "limit_order_book_test_temp";

    static const fs::path CONST_TEST_APP_ORDER_FILE_PATH =
        CONST_APP_DIR / "Orders.txt";

    static const fs::path CONST_TEST_APP_INITIAL_ORDER_FILE_PATH =
        CONST_APP_DIR / "initialOrders.txt";
}