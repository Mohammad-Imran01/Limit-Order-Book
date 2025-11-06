#include "./Generate_Orders/GenerateOrders.hpp"
#include "./Process_Orders/OrderPipeline.hpp"
#include "./Limit_Order_Book/Book.hpp"
#include "./Limit_Order_Book/Limit.hpp"
#include "./Limit_Order_Book/Order.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <memory>
#include "./constants/common.hpp"

int main() {
    return 0;//RUN_ALL_TESTS();
    auto book = std::make_shared<Book>();

    OrderPipeline orderPipeline(book);

    GenerateOrders generateOrders(book);

    generateOrders.createInitialOrders(90000, 300);
    // generateOrders.clearOrders(Im::CONST_APP_DIR / "initialOrders.txt");

    // orderPipeline.processOrdersFromFile((Im::CONST_APP_DIR / "initialOrders.txt").string());

    generateOrders.createOrders(50000);


    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // orderPipeline.processOrdersFromFile((Im::CONST_APP_DIR / "Orders.txt").string());

    // Stop measuring time
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "Time taken to process orders: " << duration.count() << " milliseconds" << std::endl;


    generateOrders.clearOrders(Im::CONST_APP_DIR / "initialOrders.txt");
    generateOrders.clearOrders(Im::CONST_APP_DIR / "Orders.txt");

    orderPipeline.processOrdersFromFile((Im::CONST_APP_DIR / "initialOrders.txt").string());
    orderPipeline.processOrdersFromFile((Im::CONST_APP_DIR / "Orders.txt").string());

    return 0;
}