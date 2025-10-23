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

int main()
{
    auto book = std::make_shared<Book>();
    {
        OrderPipeline orderPipeline(book);

        // GenerateOrders generateOrders(book);

        // generateOrders.createInitialOrders(10000, 300);

        orderPipeline.processOrdersFromFile((Im::CONST_APP_DIR / "initialOrders.txt").string());

        // generateOrders.createOrders(5000000);

        // Start measuring time
        // auto start = std::chrono::high_resolution_clock::now();

        // orderPipeline.processOrdersFromFile((Im::CONST_APP_DIR / "orders.txt").string());

        // // Stop measuring time
        // auto stop = std::chrono::high_resolution_clock::now();

        // // Calculate the duration
        // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        // std::cout << "Time taken to process orders: " << duration.count() << " milliseconds" << std::endl;
    }
    return 0;
}