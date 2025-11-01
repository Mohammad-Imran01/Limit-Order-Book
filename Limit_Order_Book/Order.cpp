#include "Order.hpp"
#include "Limit.hpp"
#include <iostream>

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
    auto prev = prevOrder.lock();

    // 1. Update the previous node's next pointer
    if (prev) {
        prev->nextOrder = nextOrder;
    } else {
        if (auto parLimit = parentLimit.lock()) {
            parLimit->setHeadOrder(nextOrder);
        }
    }

    // 2. Update the next node's previous pointer
    if (nextOrder) {
        nextOrder->setPrevOrder(prevOrder);
    } else {
        if (auto parLimit = parentLimit.lock()) {
            parLimit->setTailOrder(prevOrder);
        }
    }

    // 3. Update the Limit's volume and size (Must happen AFTER links are fixed)
    if (auto parLimit = parentLimit.lock()) {
        parLimit->totalVolume -= shares;
        parLimit->size -= 1;
    }

    // 4. Detach current node's links to ensure destruction safety
    nextOrder = nullptr;
    prevOrder = std::weak_ptr<Order>{};
}

// Execute head order (FIXED: Uses lock() for safe Limit access and uses setters)
void Order::execute() {
    // 1. Safely acquire a shared pointer to the parent limit
    if (auto parLimit = parentLimit.lock()) {
        parLimit->setHeadOrder(nextOrder);

        // 3. Update links in the next order
        if (nextOrder) {
            // Set next order's previous to null (empty weak_ptr)
            nextOrder->setPrevOrder(std::weak_ptr<Order>{});
        } else {
            // If it was the only order, update Limit's tail to null
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

    // NOTE: Changing limitPrice requires re-insertion in Limit tree
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
