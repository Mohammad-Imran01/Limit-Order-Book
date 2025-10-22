#ifndef ORDER_HPP
#define ORDER_HPP

#include <memory>

class Limit;

class Order : public std::enable_shared_from_this<Order>
{
private:
    int idNumber;
    bool buyOrSell;
    int shares;
    int limitPrice;

    std::weak_ptr<Limit> parentLimit; // weak to avoid circular reference
    std::weak_ptr<Order> prevOrder;   // weak to break circular chain
    std::shared_ptr<Order> nextOrder; // shared only forward

    friend class Limit;

public:
    Order(int _idNumber, bool _buyOrSell, int _shares, int _limitPrice);

    // --- Getters ---
    int getOrderId();
    bool getBuyOrSell();
    int getShares();
    int getLimitPrice();
    std::shared_ptr<Limit> getParentLimit() const; // lock weak_ptr
    std::shared_ptr<Order> getNextOrder() const;
    std::shared_ptr<Order> getPrevOrder() const;

    // --- Setters ---
    void setParentLimit(const std::weak_ptr<Limit> & = std::weak_ptr<Limit>());
    void setNextOrder(const std::weak_ptr<Order> & = std::weak_ptr<Order>());
    void setPrevOrder(const std::weak_ptr<Order> & = std::weak_ptr<Order>());

    // --- Operations ---
    void partiallyFillOrder(int orderedShares);
    void cancel();
    void execute();
    void modifyOrder(int newShares, int newLimitPrice);
    void setShares(int newShares);

    // --- Debug ---
    void print() const;
};

#endif
