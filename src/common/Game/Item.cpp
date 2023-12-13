#include "Item.h"

Item::Item() : index_{0} {}

int Item::getIndex() const {
    return index_;
}

bool Item::isEmpty() const {
    return index_ == 0;
}
