#pragma once

#include <string>

struct Product {
    int id;
    std::string name;
    int quantity;

    Product(int i, std::string n, int q): id(i), name(n), quantity(q) {}
};

