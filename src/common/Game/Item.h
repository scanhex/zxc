#pragma once

class Item {
private:
    int index_;  // 0 <-> empty item
public:
    Item();

    [[nodiscard]] int getIndex() const;

    [[nodiscard]] bool isEmpty() const;
};
