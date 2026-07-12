#include "inventory.hpp"

#include <cassert>
#include <sstream>
#include <string>

namespace {

void test_change_quantity_success() {
    Inventory inv;

    inv.add(2, "banana", 5);

    const bool changed = inv.change_quantity(2, 30);

    assert(changed);

    std::ostringstream out;
    inv.print_all(out);

    assert(out.str() == "ID: 2, name: banana, quantity: 30\n");
}

void test_remove_stock_success() {
    Inventory inv;

    inv.add(2, "banana", 40);

    const bool changed = inv.remove_stock(2, 7);
    assert(changed);

    std::ostringstream out;
    inv.print_all(out);

    assert(out.str() == "ID: 2, name: banana, quantity: 33\n");
}

void test_remove_stock_not_enough() {
    Inventory inv;
    inv.add(3, "coffee", 20);

    const bool removed = inv.remove_stock(3, 100);

    assert(!removed);

    std::ostringstream out;
    inv.print_all(out);

    assert(out.str() == "ID: 3, name: coffee, quantity: 20\n");
}
}

int main() {
    test_change_quantity_success();
    test_remove_stock_success();
    test_remove_stock_not_enough();

    return 0;
}

