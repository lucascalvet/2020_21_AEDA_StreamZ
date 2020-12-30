#include "product.h"

//static member of product to keep track of the id's
unsigned Product::counter = 1;

/**
 * Empty constructor of a Product
 */
Product::Product() {
    price = 0;
    stock = 0;
    id = 0;
}

/**
 * Constructor for a product
 *
 * @param price the price of the product
 * @param stock the stock of the product available
 */
Product::Product(unsigned int price, unsigned int stock) {
    this->price = price;
    this-> stock = stock;
    id = counter++;
}

/**
 * Get the product stock
 *
 * @return the product stock
 */
unsigned Product::getStock() const {
    return stock;
}

/**
 * Get the product price
 *
 * @return the product price
 */
unsigned Product::getPrice() const {
    return price;
}

/**
 * Changes the stock of a product with the correspondant stock passed
 *
 * @param stock the stock to replace the current one
 */
void Product::setStock(unsigned stock) {
    this->stock = stock;
}

/**
 * Get the id of the product
 *
 * @return the unique id of the product
 */
unsigned Product::getId() const {
    return id;
}

/**
 * Operator == for product
 *
 * @param p the product to compare with
 * @return true if id's are the same, false otherwise
 */
bool Product::operator==(const Product &p) const {
    return id == p.getId();
}
