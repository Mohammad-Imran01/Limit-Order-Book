#ifndef LIMIT_HPP
#define LIMIT_HPP

#include <memory>

class Order;

class Limit : public std::enable_shared_from_this<Limit>
{
private:
    int m_height;
    int limitPrice;
    int size;
    int totalVolume;
    bool buyOrSell;

    std::weak_ptr<Limit> parent;
    std::shared_ptr<Limit> leftChild;
    std::shared_ptr<Limit> rightChild;

    std::shared_ptr<Order> headOrder;
    std::shared_ptr<Order> tailOrder;

    friend class Order;

public:
    // ---- Constructor & Destructor ----
    Limit(int _limitPrice, bool _buyOrSell, int _size = 0, int _totalVolume = 0);

    // ---- Getters ----
    std::shared_ptr<Order> getHeadOrder() const;
    std::shared_ptr<Order> getTailOrder() const;

    void calculateAndSetHeight();
    int height() const { return m_height; }
    int getLimitPrice();
    int getSize();
    int getTotalVolume();
    bool getBuyOrSell();

    std::weak_ptr<Limit> getParent() const;
    std::shared_ptr<Limit> getLeftChild() const;
    std::shared_ptr<Limit> getRightChild() const;

    // ---- Setters ----
    void setParent(const std::weak_ptr<Limit>& newParent = std::weak_ptr<Limit>{});
    void setLeftChild(const std::shared_ptr<Limit>& newLeftChild);
    void setRightChild(const std::shared_ptr<Limit>& newRightChild);

    void setHeadOrder(const std::weak_ptr<Order>& newRightChild = std::weak_ptr<Order>{});
    void setTailOrder(const std::weak_ptr<Order>& newRightChild = std::weak_ptr<Order>{});

    // ---- Order Handling ----
    void append(const std::shared_ptr<Order>& _order);
    void partiallyFillTotalVolume(int orderedShares);

    // ---- Debug / Printing ----
    void printForward() const;
    void printBackward() const;
    void print() const;
};

#endif // LIMIT_HPP
