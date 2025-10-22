#include "Order.hpp"
#include "Limit.hpp"
#include <iostream>

Order::Order(int _idNumber, bool _buyOrSell, int _shares, int _limitPrice)
    : idNumber(_idNumber),
      buyOrSell(_buyOrSell),
      shares(_shares),
      limitPrice(_limitPrice)
{
}

// --- Getters ---
int Order::getOrderId() { return idNumber; }
bool Order::getBuyOrSell() { return buyOrSell; }
int Order::getShares() { return shares; }
int Order::getLimitPrice() { return limitPrice; }
std::shared_ptr<Limit> Order::getParentLimit() const { return parentLimit.lock(); }
std::shared_ptr<Order> Order::getNextOrder() const { return nextOrder; }
std::shared_ptr<Order> Order::getPrevOrder() const { return prevOrder.lock(); }

// --- Setters ---
void Order::setParentLimit(const std::weak_ptr<Limit> &limitPar)
{
    parentLimit = limitPar;
}
void Order::setNextOrder(const std::weak_ptr<Order> &orderNext)
{
    nextOrder = orderNext.lock();
}
void Order::setPrevOrder(const std::weak_ptr<Order> &orderPrev)
{
    prevOrder = orderPrev;
}

// --- Core Logic ---
void Order::partiallyFillOrder(int orderedShares)
{
    // if (orderedShares <= 0) return;

    // if (orderedShares > shares)
    //     orderedShares = shares;

    shares -= orderedShares;

    if (auto parent = parentLimit.lock())
    {
        parent->partiallyFillTotalVolume(orderedShares);
    }
}

// Cancel order safely
void Order::cancel()
{
    if (prevOrder.lock())
    {
        if (auto par = parentLimit.lock())
            par->setHeadOrder(nextOrder);
    }
    else
    {
        if (auto prev = prevOrder.lock())
            prev->setNextOrder(nextOrder);
    } // prev or par

    if (nextOrder)
        nextOrder->setPrevOrder(prevOrder);
    else
    {
        if (auto parLimit = parentLimit.lock())
            parLimit->setTailOrder(prevOrder);
    } // next or par

    if (auto parLimit = parentLimit.lock())
    {
        parLimit->totalVolume -= shares;
        parLimit->size -= 1;
    }
}

// Execute the order (remove head)
void Order::execute()
{
    auto parLimit = getParentLimit();
    parLimit->setHeadOrder(nextOrder);

    if (nextOrder)
    {
        nextOrder->setPrevOrder();
    }
    else
    {
        if (parLimit)
            parLimit->setTailOrder();
    }

    setNextOrder();
    setPrevOrder();

    if (parLimit)
    {
        parLimit->totalVolume -= shares;
        parLimit->size -= 1;
    }
}

// Modify order size and limit
void Order::modifyOrder(int newShares, int newLimitPrice)
{
    shares = newShares;
    limitPrice = newLimitPrice;

    // NOTE: Changing limitPrice requires re-insertion in Limit tree
}

// Set new shares only
void Order::setShares(int newShares)
{
    shares = newShares;
}

// --- Debug ---
void Order::print() const
{
    std::cout << "Order ID: " << idNumber
              << ", Type: " << (buyOrSell ? "Buy" : "Sell")
              << ", Shares: " << shares
              << ", Limit: " << limitPrice;

    if (auto parent = parentLimit.lock())
        std::cout << ", Parent Limit: " << parent->getLimitPrice();

    std::cout << std::endl;
}
