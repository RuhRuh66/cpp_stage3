#pragma once

#include "product.hpp"
#include <vector>
#include <string>
#include <ostream>



class Inventory {
private:
    std::vector<Product> products_;

public:
    void print_all(std::ostream& out) const;
    bool save_to_file(const std::string& filename) const;
    bool load_from_file(const std::string& filename);
    void add(int id, const std::string& name, int quantity);
    bool change_quantity(int id, int quantity);
    bool add_stock(int id, int quantitiy);
    bool remove_stock(int id, int amount);



};