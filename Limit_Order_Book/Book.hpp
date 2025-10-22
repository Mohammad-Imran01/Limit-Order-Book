#ifndef BOOK_HPP
#define BOOK_HPP

#include <unordered_map>
#include <vector>
#include <random>
#include <unordered_set>

class Limit;
class Order;

class Book {
private:

    std::shared_ptr<Limit> buyTree;
    std::shared_ptr<Limit> sellTree;
    std::shared_ptr<Limit> lowestSell;
    std::shared_ptr<Limit> highestBuy;

    std::shared_ptr<Limit> stopBuyTree;
    std::shared_ptr<Limit> stopSellTree;
    std::shared_ptr<Limit> highestStopSell;
    std::shared_ptr<Limit> lowestStopBuy;

    std::unordered_map<int, std::shared_ptr<Order>> orderMap;
    std::unordered_map<int, std::shared_ptr<Limit>> limitBuyMap;
    std::unordered_map<int, std::shared_ptr<Limit>> limitSellMap;
    std::unordered_map<int, std::shared_ptr<Limit>> stopMap;

    void addLimit(int limitPrice, bool buyOrSell);
    void addStop(int stopPrice, bool buyOrSell);
    std::shared_ptr<Limit> insert(std::shared_ptr<Limit> root, std::shared_ptr<Limit> limit, std::shared_ptr<Limit> parent=nullptr);
    std::shared_ptr<Limit> insertStop(std::shared_ptr<Limit> root, std::shared_ptr<Limit> limit, std::shared_ptr<Limit> parent=nullptr);
    void updateBookEdgeInsert(std::shared_ptr<Limit> newLimit);
    void updateStopBookEdgeInsert(std::shared_ptr<Limit> newStop);
    void updateBookEdgeRemove(std::shared_ptr<Limit> limit);
    void updateStopBookEdgeRemove(std::shared_ptr<Limit> stopLevel);
    void changeBookRoots(std::shared_ptr<Limit> limit);
    void changeStopBookRoots(std::shared_ptr<Limit> stopLevel);
    void deleteLimit(std::shared_ptr<Limit> limit);
    void deleteStopLevel(std::shared_ptr<Limit> limit);
    void deleteFromOrderMap(int orderId);
    void deleteFromLimitMaps(int LimitPrice, bool buyOrSell);
    void deleteFromStopMap(int StopPrice);
    int limitOrderAsMarketOrder(int orderId, bool buyOrSell, int shares, int limitPrice);
    int stopOrderAsMarketOrder(int orderId, bool buyOrSell, int shares, int stopPrice);
    int existingOrderAsMarketOrder(std::shared_ptr<Order> headOrder, bool buyOrSell);
    int stopLimitOrderAsLimitOrder(int orderId, bool buyOrSell, int shares, int limitPrice, int stopPrice);
    void executeStopOrders(bool buyOrSell);
    void stopLimitOrderToLimitOrder(std::shared_ptr<Order> headOrder, bool buyOrSell);
    void marketOrderHelper(int orderId, bool buyOrSell, int shares);

    // Functions to balance AVL tree
    int limitHeightDifference(std::shared_ptr<Limit> limit);
    std::shared_ptr<Limit> rr_rotate(std::shared_ptr<Limit> limit);
    std::shared_ptr<Limit> ll_rotate(std::shared_ptr<Limit> limit);
    std::shared_ptr<Limit> lr_rotate(std::shared_ptr<Limit> limit);
    std::shared_ptr<Limit> rl_rotate(std::shared_ptr<Limit> limit);
    std::shared_ptr<Limit> balance(std::shared_ptr<Limit> limit);
    std::shared_ptr<Limit> rr_rotateStop(std::shared_ptr<Limit> limit);
    std::shared_ptr<Limit> ll_rotateStop(std::shared_ptr<Limit> limit);
    std::shared_ptr<Limit> lr_rotateStop(std::shared_ptr<Limit> limit);
    std::shared_ptr<Limit> rl_rotateStop(std::shared_ptr<Limit> limit);
    std::shared_ptr<Limit> balanceStop(std::shared_ptr<Limit> limit);

public:
    Book();
    ~Book();

    // Counts used in order book perforamce visualisations
    int executedOrdersCount=0;
    int AVLTreeBalanceCount=0;

    // Getter and setter functions
    std::shared_ptr<Limit> getBuyTree() const;
    std::shared_ptr<Limit> getSellTree() const;
    std::shared_ptr<Limit> getLowestSell() const;
    std::shared_ptr<Limit> getHighestBuy() const;
    std::shared_ptr<Limit> getStopBuyTree() const;
    std::shared_ptr<Limit> getStopSellTree() const;
    std::shared_ptr<Limit> getHighestStopSell() const;
    std::shared_ptr<Limit> getLowestStopBuy() const;

    // Functions for different types of orders
    void marketOrder(int orderId, bool buyOrSell, int shares);
    void addLimitOrder(int orderId, bool buyOrSell, int shares, int limitPrice);
    void cancelLimitOrder(int orderId);
    void modifyLimitOrder(int orderId, int newShares, int newLimit);
    void addStopOrder(int orderId, bool buyOrSell, int shares, int stopPrice);
    void cancelStopOrder(int orderId);
    void modifyStopOrder(int orderId, int newShares, int newStopPrice);
    void addStopLimitOrder(int orderId, bool buyOrSell, int shares, int limitPrice, int stopPrice);
    void cancelStopLimitOrder(int orderId);
    void modifyStopLimitOrder(int orderId, int newShares, int newLimitPrice, int newStopPrice);

    // Functions that needed to be public for testing purposes
    int getLimitHeight(std::shared_ptr<Limit> limit) const;
    std::shared_ptr<Order> searchOrderMap(int orderId) const;
    std::shared_ptr<Limit> searchLimitMaps(int limitPrice, bool buyOrSell) const;
    std::shared_ptr<Limit> searchStopMap(int stopPrice) const;

    // Functions for visualising the order book
    void printLimit(int limitPrice, bool buyOrSell) const;
    void printOrder(int orderId) const;
    void printBookEdges() const;
    void printOrderBook() const;
    std::vector<int> inOrderTreeTraversal(std::shared_ptr<Limit> root) const;
    std::vector<int> preOrderTreeTraversal(std::shared_ptr<Limit> root) const;
    std::vector<int> postOrderTreeTraversal(std::shared_ptr<Limit> root) const;

    // Functions and data structures needed for generating sample data
    std::shared_ptr<Order> getRandomOrder(int key, std::mt19937 gen) const;
    std::unordered_set<std::shared_ptr<Order>> limitOrders;
    std::unordered_set<std::shared_ptr<Order>> stopOrders;
    std::unordered_set<std::shared_ptr<Order>> stopLimitOrders;
};

#endif