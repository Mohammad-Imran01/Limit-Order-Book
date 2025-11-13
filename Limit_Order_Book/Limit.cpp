#include "Limit.hpp"
#include "Order.hpp"
#include <iostream>

Limit::Limit(int _limitPrice, bool _buyOrSell, int _size, int _totalVolume)
    :
    m_height(1),
    limitPrice(_limitPrice),
    size(_size),
    totalVolume(_totalVolume),
    buyOrSell(_buyOrSell),
    parent(),
    leftChild(nullptr),
    rightChild(nullptr),
    headOrder(nullptr),
    tailOrder(nullptr) {
}

// ---- Getters ----
void Limit::calculateAndSetHeight() {
    int l_h = leftChild ? leftChild->m_height : 0;
    int r_h = rightChild ? rightChild->m_height : 0;
    m_height = 1 + std::max(l_h, r_h);
}

std::shared_ptr<Order> Limit::getHeadOrder() const { return headOrder; }
std::shared_ptr<Order> Limit::getTailOrder() const { return tailOrder; }
int Limit::getLimitPrice() { return limitPrice; }
int Limit::getSize() { return size; }
int Limit::getTotalVolume() { return totalVolume; }
bool Limit::getBuyOrSell() { return buyOrSell; }
std::weak_ptr<Limit> Limit::getParent() const { return parent; }
std::shared_ptr<Limit> Limit::getLeftChild() const { return leftChild; }
std::shared_ptr<Limit> Limit::getRightChild() const { return rightChild; }

// ---- Setters ----

void Limit::setParent(const std::weak_ptr<Limit>& newParent) {
    parent = newParent;
}
void Limit::setLeftChild(const std::shared_ptr<Limit>& newLeftChild) {
    leftChild = newLeftChild;
    if (leftChild)
        leftChild->setParent(shared_from_this());
}

void Limit::setRightChild(const std::shared_ptr<Limit>& newRightChild) {
    rightChild = newRightChild;
    if (rightChild)
        rightChild->setParent(shared_from_this());
}

void Limit::setHeadOrder(const std::weak_ptr<Order>& newHeadOrder) {
    headOrder = newHeadOrder.lock();
}
void Limit::setTailOrder(const std::weak_ptr<Order>& newTailOrder) {
    tailOrder = newTailOrder.lock();
}

void Limit::partiallyFillTotalVolume(int orderedShares) {
    totalVolume -= orderedShares;
}

// ---- Order Handling ----

void Limit::append(const std::shared_ptr<Order>& order) {
    order->setParentLimit(shared_from_this());

    if (!headOrder) {
        headOrder = order;
        tailOrder = order;

        order->setPrevOrder(std::weak_ptr<Order>{});
        order->setNextOrder(std::weak_ptr<Order>{});
    } else {
        order->setPrevOrder(tailOrder);
        tailOrder->setNextOrder(order);
        tailOrder = order;
        tailOrder->setNextOrder();
    }

    size++;
    totalVolume += order->getShares();
}
// ---- Print Helpers ----

void Limit::printForward() const {
    auto current = headOrder;
    while (current) {
        std::cout << current->getOrderId() << " ";
        current = current->nextOrder;
    }
    std::cout << std::endl;
}

void Limit::printBackward() const {
    auto current = tailOrder;
    while (current) {
        std::cout << current->getOrderId() << " ";
        current = current->prevOrder.lock();
    }
    std::cout << std::endl;
}

void Limit::print() const {
    std::cout << "Limit Price: " << limitPrice
        << ", Volume: " << totalVolume
        << ", Size: " << size
        << std::endl;
}
