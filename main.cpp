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
    auto book = std::make_shared<Book>();

    OrderPipeline orderPipeline(book);
    GenerateOrders generateOrders(book);

    generateOrders.createInitialOrders(500, 300);
    generateOrders.createOrders(500);

    std::cout << "Created initial and main orders. successfully!";

    auto start = std::chrono::high_resolution_clock::now();

    orderPipeline.processOrdersFromFile((Im::CONST_APP_DIR / "initialOrders.txt").string());
    orderPipeline.processOrdersFromFile((Im::CONST_APP_DIR / "Orders.txt").string());

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "Time taken to process orders: " << duration.count() << " milliseconds" << std::endl;


    generateOrders.clearOrders(Im::CONST_APP_DIR / "initialOrders.txt");
    generateOrders.clearOrders(Im::CONST_APP_DIR / "Orders.txt");

    return 0;
}