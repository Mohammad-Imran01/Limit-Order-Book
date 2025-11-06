#include "Order.hpp"
#include "Limit.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <utility>

Order::Order(int _idNumber, bool _buyOrSell, int _shares, int _limitPrice)
    : idNumber(_idNumber),
    buyOrSell(_buyOrSell),
    shares(_shares),
    limitPrice(_limitPrice) {
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
void Order::setParentLimit(const std::weak_ptr<Limit>& limitPar) {
    parentLimit = limitPar;
}
void Order::setNextOrder(const std::weak_ptr<Order>& orderNext) {
    nextOrder = orderNext.lock();
}
void Order::setPrevOrder(const std::weak_ptr<Order>& orderPrev) {
    prevOrder = orderPrev;
}

// --- Core Logic ---
void Order::partiallyFillOrder(int orderedShares) {
    // if (orderedShares <= 0) return;

    // if (orderedShares > shares)
    //     orderedShares = shares;

    shares -= orderedShares;

    if (auto parent = parentLimit.lock()) {
        parent->partiallyFillTotalVolume(orderedShares);
    }
}


void Order::cancel() {
    if (auto parLimit = parentLimit.lock()) {

        if (prevOrder.lock() == nullptr) {
            parLimit->headOrder = nextOrder;
        } else {
            if (auto prev = prevOrder.lock()) {
                prev->nextOrder = nextOrder;
            }
        }

        if (!nextOrder) {
            parLimit->tailOrder = prevOrder.lock();
        } else {
            nextOrder->prevOrder = prevOrder;
        }

        parLimit->totalVolume -= shares;
        parLimit->size -= 1;

        // setNextOrder();
        // setPrevOrder();
        // setParentLimit();
    }
}


void Order::execute() {
    if (auto parLimit = parentLimit.lock()) {
        parLimit->setHeadOrder(nextOrder);

        if (nextOrder) {
            nextOrder->setPrevOrder(std::weak_ptr<Order>{});
        } else {
            parLimit->setTailOrder(std::weak_ptr<Order>{});
        }
        parLimit->totalVolume -= shares;
        parLimit->size -= 1;
    }

    nextOrder = nullptr;
    prevOrder = std::weak_ptr<Order>{};
}

// Modify order size and limit
void Order::modifyOrder(int newShares, int newLimitPrice) {
    shares = newShares;
    limitPrice = newLimitPrice;
}

// Set new shares only
void Order::setShares(int newShares) {
    shares = newShares;
}

// --- Debug ---
void Order::print() const {
    std::cout << "Order ID: " << idNumber
        << ", Type: " << (buyOrSell ? "Buy" : "Sell")
        << ", Shares: " << shares
        << ", Limit: " << limitPrice;

    if (auto parent = parentLimit.lock())
        std::cout << ", Parent Limit: " << parent->getLimitPrice();

    std::cout << std::endl;
}
