#include <iostream>
#include <fstream>

#include "inventory.hpp"

void Inventory::add(int id, const std::string& name, int quantity) {
    Product pro(id, name, quantity);
    products_.push_back(pro);
}

bool Inventory::save_to_file(const std::string& filename) const {
    std::ofstream out(filename);

    if (!out) {
        return false;
    }

    for (const Product& pro: products_) {
        out << pro.id << ',' << pro.name << ',' << pro.quantity << '\n';
    }

    return true;

}

bool Inventory::load_from_file(const std::string& filename) {
    std::ifstream in(filename);

    if (!in) {
        return false;
    }

    products_.clear();

    std::string line;

    while (std::getline(in, line)) {
        const std::size_t first_comma = line.find(',');
        const std::size_t second_comma = line.find(',', first_comma + 1);

        const std::string id_text = line.substr(0, first_comma);
        const std::string name = line.substr(first_comma+1,
                         second_comma - first_comma -1);
        const std::string quantity_text = line.substr(second_comma+1);

        int id = std::stoi(id_text);
        int quantity = std::stoi(quantity_text);

        add(id, name, quantity);
    }   

    return true;

}

void Inventory::print_all(std::ostream& out) const {
    for (const Product& pro: products_) {
        out << "ID: " << pro.id << ", name: " << pro.name << ", quantity: " << pro.quantity << '\n';
    }

}
bool Inventory::change_quantity(int id, int quantity) {
    for (Product& pro: products_) {
        if (pro.id == id) {
            pro.quantity = quantity;
            return true;
        }
    }
    return false;
}

bool Inventory::add_stock(int id, int quantity) {
    for (Product& pro: products_) {
        if (pro.id == id) {
            pro.quantity += quantity;
            return true;
        }
    }
    return false;
}

bool Inventory::remove_stock(int id, int amount) {
    if (amount <= 0) {
        return false;
    }

    for (Product& pro: products_) {
        if (pro.id == id) {
            if (pro.quantity < amount) {
                return false;
            }
            pro.quantity -= amount;
            return true;
        }

    }
    return false;

}

    


