#ifndef STREAMZ_AEDA_2020_21_PRODUCT_H
#define STREAMZ_AEDA_2020_21_PRODUCT_H

#include <string>

class Product {
private:
    static unsigned counter;
    unsigned id;
    unsigned price;
    unsigned stock;
public:
    Product();
    Product(unsigned price, unsigned stock);
    unsigned getStock() const;
    unsigned getPrice() const;
    unsigned getId() const;
    std::string getInfo() const;
    void setStock(unsigned stock);
    bool operator==(const Product &p) const;
};

#endif //STREAMZ_AEDA_2020_21_PRODUCT_H
