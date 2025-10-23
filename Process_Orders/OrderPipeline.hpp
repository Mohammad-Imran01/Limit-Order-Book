#ifndef ORDERPIPELINE_HPP
#define ORDERPIPELINE_HPP

#include <string>
#include <unordered_map>
#include <string_view>
#include <sstream>
#include <memory>

class Book;

class OrderPipeline {
private:
    std::shared_ptr<Book> book;

    using OrderFunction = void(OrderPipeline::*)(std::istringstream&);
    std::unordered_map<std::string, OrderFunction> orderFunctions;

    void processMarketOrder(std::istringstream& iss);
    void processAddLimitOrder(std::istringstream& iss);
    void processCancelLimitOrder(std::istringstream& iss);
    void processModifyLimitOrder(std::istringstream& iss);
    void processAddStopOrder(std::istringstream& iss);
    void processCancelStopOrder(std::istringstream& iss);
    void processModifyStopOrder(std::istringstream& iss);
    void processAddStopLimitOrder(std::istringstream& iss);
    void processCancelStopLimitOrder(std::istringstream& iss);
    void processModifyStopLimitOrder(std::istringstream& iss);

public:
    OrderPipeline(std::shared_ptr<Book> book);
    void processOrdersFromFile(const std::string& filename);
};

#endif