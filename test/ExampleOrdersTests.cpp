#include "../Limit_Order_Book/Limit.hpp"
#include "../Limit_Order_Book/Order.hpp"
#include "../Limit_Order_Book/Book.hpp"
#include "../Process_Orders/OrderPipeline.hpp"
#include "../Generate_Orders/GenerateOrders.hpp"

#include <gtest/gtest.h>
#include <memory>

struct ExampleOrdersTests : public ::testing::Test
{
    std::shared_ptr<Book> book;
    std::unique_ptr<OrderPipeline> orderPipeline;
    std::unique_ptr<GenerateOrders> generateOrders;

    virtual void SetUp() override
    {
        book = std::make_shared<Book>();
        orderPipeline = std::make_unique<OrderPipeline>(book);
        generateOrders = std::make_unique<GenerateOrders>(book);
    }

    virtual void TearDown() override
    {
        // delete generateOrders;
        // delete orderPipeline;
        // delete book;
    }
};

TEST_F(ExampleOrdersTests, CreateInitialOrdersTest)
{
    generateOrders->createInitialOrders(10000, 300);
}

TEST_F(ExampleOrdersTests, ProcessInitialOrdersTest)
{
    orderPipeline->processOrdersFromFile("C:/Users/benja/Documents/Limit_order_book/initialOrders.txt");
}

TEST_F(ExampleOrdersTests, CreateOrdersTest)
{
    orderPipeline->processOrdersFromFile("C:/Users/benja/Documents/Limit_order_book/initialOrders.txt");
    generateOrders->createOrders(100000);
}