#include <iostream>
#include <string>
#include <vector>  // For vector usage
#include <fstream> // For file output
#include <ctime>   // For timing
using namespace std;

// Specimen Node
struct Specimen {
    string name;       // Specimen name
    float price;       // Price per unit
    string UPC;        // Unique Product Code
    Specimen* left;    // Left child for tree
    Specimen* right;   // Right child

    Specimen(string u, string n, float p) : UPC(u), name(n), price(p), left(nullptr), right(nullptr) {}
};

// SpecimenInventory Class
class Specimens {
private:
    Specimen* root;

    Specimen* insert(Specimen* node, string UPC, string name, float price) {
        if (node == nullptr) return new Specimen(UPC, name, price);
        if (UPC < node->UPC) 
            node->left = insert(node->left, UPC, name, price);
        else if (UPC > node->UPC) 
            node->right = insert(node->right, UPC, name, price);
        return node;
    }

    void inOrder(Specimen* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << "UPC: " << node->UPC << ", Name: " << node->name << ", Price: $" << node->price << endl;
        inOrder(node->right);
    }

    int search(Specimen* node, string UPC, int& steps) {
        if (node == nullptr) {
            cout << "Null node." << endl;
            return steps;
        }
        steps++;  // Increment step counter for each node visited
        cout << "Step " << steps << ": Visiting node with UPC: " << node->UPC << endl;
        if (UPC == node->UPC) return steps;
        if (UPC < node->UPC) return search(node->left, UPC, steps);
        return search(node->right, UPC, steps);
    }

public:
    Specimens() : root(nullptr) {}

    void addSpecimen(string UPC, string name, float price) {
        root = insert(root, UPC, name, price);
    }

    void displayInventory() {
        cout << "Inventory List (sort by UPC):" << endl;
        inOrder(root);
    }

    double findSpecimenSteps(string UPC, int& steps) {
        clock_t start = clock(); // Start timing
        int traversalSteps = search(root, UPC, steps);
        clock_t end = clock();   // End timing

        double duration = 1000.0 * (end - start) / CLOCKS_PER_SEC; // Convert to milliseconds
        cout << "Search completed in " << traversalSteps << " steps (Time: " << duration << " ms)" << endl;

        return duration;
    }
};

// Main function
int main() {
    Specimens inventory;

    vector<pair<string, pair<string, float>>> Collection = {
        {"000", {"Free", 0.00}},
        {"001", {"Quartz", 12.99}},
        {"002", {"Amethyst", 25.49}},
        {"003", {"Calcite", 8.75}},
        {"004", {"Pyrite", 15.30}},
        {"005", {"Topaz", 20.00}},
        {"006", {"Malachite", 18.60}},
        {"007", {"Garnet", 22.45}},
        {"008", {"Obsidian", 9.80}},
        {"009", {"Fluorite", 14.25}},
        {"010", {"Aragonite", 27.20}}       
    };

    for (const auto& specimen : Collection) {
        inventory.addSpecimen(specimen.first, specimen.second.first, specimen.second.second);
    }

    inventory.displayInventory();

    vector<string> searchUPCs = {"000", "001", "002", "003", "004", "005","006","007","008","009","010","011"}; 
    
    ofstream outputFile("search_results_steps.csv");
    outputFile << "UPC,Steps,Time(ms)\n";
    for (const string& UPC : searchUPCs) {
        int steps = 0;
        double time = inventory.findSpecimenSteps(UPC, steps);
        outputFile << UPC << "," << steps << "," << time << "\n";
    }
    outputFile.close();

    cout << "\nSearch results saved to 'search_results_steps.csv'." << endl;
    return 0;
}
