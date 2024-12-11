#include <iostream>
#include <string>
#include <vector>  // For vector usage
#include <fstream> // For file output
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

    //******************************************************************/
    // insert - Helper function to insert a node into the tree
    // arguments - node (current node), UPC (str), name (str), price (fl)
    // returns - ptr to the updated node
    //******************************************************************/
    Specimen* insert(Specimen* node, string UPC, string name, float price) {
        if (node == nullptr) return new Specimen(UPC, name, price);
        if (UPC < node->UPC) node->left = insert(node->left, UPC, name, price);
        else if (UPC > node->UPC) node->right = insert(node->right, UPC, name, price);
        return node;
    }

    //******************************************************************/
    // inOrder - Helper function for in-order traversal
    // arguments - node (current node)
    // returns - void
    //******************************************************************/
    void inOrder(Specimen* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << "UPC: " << node->UPC << ", Name: " << node->name << ", Price: $" << node->price << endl;
        inOrder(node->right);
    }

    //******************************************************************/
    // search - Helper function to search for a node by UPC
    // arguments - node (current node), UPC (str), steps (int ref)
    // returns - ptr to the found node or nullptr if not found
    //******************************************************************/
    Specimen* search(Specimen* node, string UPC, int& steps) {
        if (node == nullptr) {
            cout << "Null node." << endl;
            return node;
        }
        steps++;  // Iterate step counter for each node visited
        cout << "Step " << steps << ": Visiting node with UPC: " << node->UPC << endl;
        if (UPC == node->UPC) return node;
        if (UPC < node->UPC) return search(node->left, UPC, steps);
        return search(node->right, UPC, steps);
    }

public:
    //*****************************************************************/
    // addSpecimen - Inserts a new Specimen into the inventory
    // arguments - UPC (string), name (string), price (float)
    // returns - void
    //*****************************************************************/
    void addSpecimen(string UPC, string name, float price) {
        root = insert(root, UPC, name, price);
    }

    //*****************************************************************/
    // displayInventory - Displays the Specimen inventory in sorted order
    // arguments - none
    // returns - void
    //*****************************************************************/
    void displayInventory() {
        cout << "Inventory List (sort by UPC):" << endl;
        inOrder(root);
    }

    //*****************************************************************/
    // findSpecimenWithSteps - Searches for a Specimen and counts traversal steps
    // arguments - UPC (string)
    // returns - pair<bool, int> indicating if the Specimen was found and steps taken
    //*****************************************************************/
    pair<bool, int> findSpecimenWithSteps(string UPC) {
        int steps = 0;
        cout << "\nSearching for Specimen - UPC: " << UPC << endl;
        Specimen* result = search(root, UPC, steps);
        if (result) {
            cout << "Found Specimen - UPC: " << result->UPC; 
            cout << ", Name: " << result->name << ", Price: $";
            cout << result->price << endl;
            return {true, steps};
        } else {
            cout << "Specimen - UPC " << UPC << " not found!" << endl;
            return {false, steps};
        }
    }
};

// Main function
int main() {
    Specimens inventory;

    // Dataset with at least 10 Specimens
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
        {"010", {"Citrine", 17.99}}        
    };

    // Add Specimens to the inventory
    for (const auto& specimen : Collection) {
        inventory.addSpecimen(specimen.first, specimen.second.first, specimen.second.second);
    }

    // Display the inventory
    inventory.displayInventory();

    // Perform searches and record steps
    vector<string> searchUPCs = {"002", "004", "007", "011", "A", "", "001"};  // Includes invalid cases
    
    // save to csv file to analyze
    ofstream outputFile("search_results_steps.csv");
    outputFile << "UPC,Found,Steps\n";
    for (const string& UPC : searchUPCs) {
        auto [found, steps] = inventory.findSpecimenWithSteps(UPC);
        outputFile << UPC << "," << (found ? "Yes" : "No") << "," << steps << "\n";
    }
    outputFile.close();

    cout << "\nSearch results saved to 'search_results_steps.csv'." << endl;
    return 0;
}
