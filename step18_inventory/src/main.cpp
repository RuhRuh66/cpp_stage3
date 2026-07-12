#include "inventory.hpp"

#include <iostream>

int main() {
    Inventory inv;
    if (!inv.load_from_file("data/inventory.csv")){
        std::cerr << "failed to load inventory\n";
        return 1;
    }

    inv.print_all(std::cout);

    return 0;

}
