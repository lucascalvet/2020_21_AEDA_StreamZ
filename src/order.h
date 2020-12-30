#ifndef STREAMZ_AEDA_2020_21_ORDER_H
#define STREAMZ_AEDA_2020_21_ORDER_H

#include <string>

class Order {
private:
    std::string viewer_nickname;
    unsigned quantity;
    unsigned priority;
    unsigned product_id;
public:
    Order();
    Order(unsigned quantity, unsigned priority, std::string nickname);
    unsigned getQuantity() const;
    unsigned getPriority() const;
    std::string getViewerNickname() const;
    std::string getInfo() const;
    bool operator<(const Order &o) const;
    bool operator==(const Order &o) const;
    void setProductId(unsigned id);
    unsigned getProductId() const;
};




#endif //STREAMZ_AEDA_2020_21_ORDER_H
