#include "order.h"
#include "exceptions.h"
#include <sstream>

using namespace std;

/**
 * Construct an order
 *
 * @param quantity the quantity of products to buy
 * @param priority the priority of the purchase (1-5)
 * @param nickname the viewer's nickname that is making the order
 */
Order::Order(unsigned int quantity, unsigned int priority, std::string nickname) {
    if(priority < 1 || priority > 5) throw InvalidPriority();

    this->quantity = quantity;
    this->priority = priority;
    this->viewer_nickname = nickname;
}

/**
 * Get the order quantity of products to buy
 *
 * @return the order quantity
 */
unsigned Order::getQuantity() const {
    return quantity;
}

/**
 * Get the order priority labeled from 1 to 5
 *
 * @return the order priority
 */
unsigned Order::getPriority() const {
    return priority;
}

/**
 * Get the viewer's nickname that made the order
 *
 * @return the order viewer's nickname
 */
string Order::getViewerNickname() const {
    return viewer_nickname;
}

/**
 * Get info about the order
 *
 * @return string the contains the info about the order
 */
std::string Order::getInfo() const {
    stringstream info;
    info << "Viewer's name: " << viewer_nickname;
    info << "\tQuantity: " << to_string(quantity);
    info << "\tPriority: " << to_string(priority);
    return info.str();
}

/**
 * < operator for Order class
 *
 * @param o the other order to compare with
 * @return true if quantity is greater, if equal quantities, true if priority is smaller, (if equal priorities true is nickname is 'smaller'), false otherwise
 */
bool Order::operator<(const Order &o) const {
    if(quantity > o.getQuantity()) return true;
    if(quantity == o.getQuantity()){
        if(priority < o.getPriority()) return true;
        if(priority == o.getPriority()) {
            if (viewer_nickname < o.getViewerNickname()) return true;
        }
    }
    return false;
}

/**
 * == operator for Order class
 *
 * @param o the other order to compare with
 * @return true if nickname, quantity and priority are all the same
 */
bool Order::operator==(const Order &o) const {
    return viewer_nickname == o.getViewerNickname() && quantity == o.getQuantity() && priority == o.getPriority();
}

/**
 * empty Order constructor
 */
Order::Order() {
    this->viewer_nickname = "";
    this->priority = 0;
    this->quantity = 0;
}
