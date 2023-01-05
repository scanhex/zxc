#include "Item.h"

Item::Item() : index_{0} {}

int Item::getIndex() { return index_; }

bool Item::isEmpty() { return index_ == 0; }
