#pragma once

class Item {
private:
    int index_; // 0 <-> empty item
public:
    Item();

    int getIndex();

    bool isEmpty();
};
