#include "Limit.hpp"
#include "Order.hpp"
#include <iostream>

Limit::Limit(int _limitPrice, bool _buyOrSell, int _size, int _totalVolume)
    : limitPrice(_limitPrice),
      size(_size),
      totalVolume(_totalVolume),
      buyOrSell(_buyOrSell),
      parent(),
      leftChild(nullptr),
      rightChild(nullptr),
      headOrder(nullptr),
      tailOrder(nullptr)
{
}

Limit::~Limit()
{
    auto sharedParent = parent.lock();

    if (sharedParent)
    {
        bool isLeftChild = (limitPrice < sharedParent->getLimitPrice());

        // ---- Case 1: Node with only one child or no child ----
        if (!leftChild)
        {
            if (isLeftChild)
                sharedParent->leftChild = rightChild;
            else
                sharedParent->rightChild = rightChild;

            if (rightChild)
                rightChild->setParent(sharedParent);
            return;
        }
        else if (!rightChild)
        {
            if (isLeftChild)
                sharedParent->leftChild = leftChild;
            else
                sharedParent->rightChild = leftChild;

            leftChild->setParent(sharedParent);
            return;
        }

        // ---- Case 2: Node with two children ----
        std::shared_ptr<Limit> temp = rightChild;
        while (temp->getLeftChild())
            temp = temp->getLeftChild();

        if (auto right = rightChild->getLeftChild())
        {
            temp->getParent().lock()->setLeftChild(temp->getRightChild());
            if (temp->getRightChild())
                temp->getRightChild()->setParent(temp->getParent());

            temp->setRightChild(rightChild);
            rightChild->setParent(temp);
        }

        temp->setParent(sharedParent);
        temp->setLeftChild(leftChild);
        leftChild->setParent(temp);

        if (isLeftChild)
            sharedParent->setLeftChild(temp);
        else
            sharedParent->setRightChild(temp);
    }
    else
    {
        // ---- Root case ----
        if (!leftChild && !rightChild)
        {
            return;
        }
        else if (!leftChild)
        {
            rightChild->setParent(std::weak_ptr<Limit>());
            return;
        }
        else if (!rightChild)
        {
            leftChild->setParent(std::weak_ptr<Limit>());
            return;
        }

        std::shared_ptr<Limit> temp = rightChild;
        while (temp->getLeftChild())
            temp = temp->getLeftChild();

        if (auto right = rightChild->getLeftChild())
        {
            temp->getParent().lock()->setLeftChild(temp->getRightChild());
            if (temp->getRightChild())
                temp->getRightChild()->setParent(temp->getParent());

            temp->setRightChild(rightChild);
            rightChild->setParent(temp);
        }

        temp->setParent(std::weak_ptr<Limit>()); // no parent
        temp->setLeftChild(leftChild);
        leftChild->setParent(temp);
    }
}

// ---- Getters ----

std::shared_ptr<Order> Limit::getHeadOrder() const { return headOrder; }
int Limit::getLimitPrice() { return limitPrice; }
int Limit::getSize() { return size; }
int Limit::getTotalVolume() { return totalVolume; }
bool Limit::getBuyOrSell() { return buyOrSell; }
std::weak_ptr<Limit> Limit::getParent() const { return parent; }
std::shared_ptr<Limit> Limit::getLeftChild() const { return leftChild; }
std::shared_ptr<Limit> Limit::getRightChild() const { return rightChild; }

// ---- Setters ----

void Limit::setParent(const std::weak_ptr<Limit> &newParent)
{
    parent = newParent;
}

void Limit::setLeftChild(const std::shared_ptr<Limit> &newLeftChild)
{
    leftChild = newLeftChild;
    if (leftChild)
        leftChild->setParent(shared_from_this());
}

void Limit::setRightChild(const std::shared_ptr<Limit> &newRightChild)
{
    rightChild = newRightChild;
    if (rightChild)
        rightChild->setParent(shared_from_this());
}

void Limit::setHeadOrder(const std::weak_ptr<Order> &newHeadOrder)
{
    headOrder = newHeadOrder.lock();
}
void Limit::setTailOrder(const std::weak_ptr<Order> &newTailOrder)
{
    tailOrder = newTailOrder.lock();
}

void Limit::partiallyFillTotalVolume(int orderedShares)
{
    totalVolume -= orderedShares;
}

// ---- Order Handling ----

void Limit::append(const std::shared_ptr<Order> &order)
{
    if (!headOrder)
    {
        headOrder = tailOrder = order;
    }
    else
    {
        tailOrder->nextOrder = order;
        order->prevOrder = tailOrder;
        order->nextOrder = nullptr;
        tailOrder = order;
    }

    size++;
    totalVolume += order->getShares();

    order->parentLimit = shared_from_this();
}

// ---- Print Helpers ----

void Limit::printForward() const
{
    auto current = headOrder;
    while (current)
    {
        std::cout << current->getOrderId() << " ";
        current = current->nextOrder;
    }
    std::cout << std::endl;
}

void Limit::printBackward() const
{
    auto current = tailOrder;
    while (current)
    {
        std::cout << current->getOrderId() << " ";
        current = current->prevOrder.lock();
    }
    std::cout << std::endl;
}

void Limit::print() const
{
    std::cout << "Limit Price: " << limitPrice
              << ", Volume: " << totalVolume
              << ", Size: " << size
              << std::endl;
}
