#include "Order.h"
#include <algorithm>

// TODO: Check if i if between 1 and 5
Order::Order(Viewer *customer, int disp, const std::string &streamer) : customer(customer), disp(disp), streamer(streamer){
}

void Order::addProduct(const Product &product) {
    if(!product.getName().empty())
        products.push_back(product);
}

double Order::getTotalPrice() const {
    double total = 0.0;
    for(const auto &p : products){
        total += p.getPrice();
    }
    return total;
}

int Order::getSize() const {
    return products.size();
}

int Order::getDisp() const {
    return disp;
}

bool Order::operator<(const Order &o) const {
    if(getSize() == o.getSize()){
        return getDisp() < o.getDisp();
    }
    return getSize() > o.getSize();
}

std::ostream &operator<<(std::ostream &os, const Order &o) {
    os << "-------------------------------------" << std::endl;
    os << o.customer->getNickname() << " " << o.getDisp() << std::endl;
    os << "- - - - - - - - - - - - - - - - - - -" << std::endl;
    for(const auto &prod : o.products){
        os << prod;
    }
    os << "Total price: " << o.getTotalPrice() << "€" << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}

std::string Order::getCustomerNickname() const {
    return customer->getNickname();
}

bool Order::operator==(const Order &o) const {
    if(getCustomerNickname()==o.getCustomerNickname() && streamer==o.streamer && disp==o.disp && products.size()==o.products.size()){
        std::vector<Product>::const_iterator it1, it2;
        for(it1 = products.cbegin(), it2 = o.products.cbegin(); it1 != products.cend() && it2 != o.products.cend(); ++it1, ++it2){
            if((*it1).getName() != (*it2).getName()){
                return false;
            }
        }
        return true;
    }
    return false;
}

void Order::removeProduct(const Product &product) {
    auto it = std::find(products.begin(), products.end(), product);
    if(it != products.end()){
        products.erase(it);
    }
}
