#include <iostream>   // This library allows us to use input/output (like cin and cout).
#include <fstream>    // This library allows us to read and write files (like inventory.txt and sales.txt).
#include <string>     // This library helps us work with strings (text).
#include <ctime>      // This library helps us work with time and date.
#include <iomanip>    // This library helps us format output (setw() and left for alignment).
using namespace std;

string inventory[100][3]; // Stores itemName, quantity, price
int inventoryCount = 0;
string sales[100][4]; // Stores itemName, quantitySold, totalPrice, timestamp
int salesCount = 0;

string getCurrentTime() { // Returns the current date and time in the format
    time_t now = time(0); // Get the current time in seconds since 1970
    struct tm ltm; // Create a structure to hold local time information
    localtime_s(&ltm, &now); // Convert the time to a local time (in struct tm format)
    char buffer[20]; // Create a buffer to store the formatted time
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", &ltm); // Format the time into a string format
    return string(buffer); // Return the formatted time as a string
}

string getDateOnly(time_t time) { // Returns the date (YYYY-MM-DD) from a given timestamp (time_t)
    struct tm ltm;
    localtime_s(&ltm, &time);
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", &ltm);
    return string(buffer);
}

void loadInventory() { // Loads inventory data from a file ("inventory.txt") into the inventory array.
    ifstream file("inventory.txt");
    if (file.is_open()) {
        while (file >> inventory[inventoryCount][0] >> inventory[inventoryCount][1] >> inventory[inventoryCount][2]) {
            inventoryCount++;
        }
        file.close();
    }
}

void saveInventory() { // Saves the current inventory data from the inventory array into a file ("inventory.txt").
    ofstream file("inventory.txt");
    for (int i = 0; i < inventoryCount; i++) {
        file << inventory[i][0] << " " << inventory[i][1] << " " << inventory[i][2] << "\n";
    }
    file.close();
}

void addStock() { // Adds or updates the stock for a specific item in the inventory.
    string itemName, quantity, price;
    cout << "Enter item name: ";
    cin >> itemName;
    cout << "Enter quantity: ";
    cin >> quantity;
    cout << "Enter price: ";
    cin >> price;

    bool exists = false;
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i][0] == itemName) {
            inventory[i][1] = quantity;
            inventory[i][2] = price;
            exists = true;
            break;
        }
    }

    if (!exists) {
        inventory[inventoryCount][0] = itemName;
        inventory[inventoryCount][1] = quantity;
        inventory[inventoryCount][2] = price;
        inventoryCount++;
    }

    saveInventory();
    cout << "Stock added successfully.\n";
}

void viewInventory() { // Displays the list of items in the inventory with their quantity and price.
    cout << left << setw(20) << "Item Name"
        << setw(10) << "Quantity"
        << setw(10) << "Price" << endl;

    for (int i = 0; i < inventoryCount; i++) {
        cout << left << setw(20) << inventory[i][0]
            << setw(10) << inventory[i][1]
            << setw(10) << inventory[i][2] << endl;
    }
}

void loadSales() { // Loads sales data from a file ("sales.txt") into the sales array.
    ifstream file("sales.txt");
    if (file.is_open()) {
        while (file >> sales[salesCount][0] >> sales[salesCount][1] >> sales[salesCount][2] >> sales[salesCount][3]) {
            salesCount++;
        }
        file.close();
    }
}

void saveSales() { // Saves the current sales data from the sales array into a file ("sales.txt").
    ofstream file("sales.txt");
    for (int i = 0; i < salesCount; i++) {
        file << sales[i][0] << " " << sales[i][1] << " " << sales[i][2] << " " << sales[i][3] << "\n";
    }
    file.close();
}

void recordSale() { // Records a sale by decreasing the stock and adding a new sale entry in the sales array.
    string itemName, quantityStr;
    cout << "Enter item name: ";
    cin >> itemName;

    int index = -1;
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i][0] == itemName) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Item not found.\n";
        return;
    }

    cout << "Enter quantity to sell: ";
    cin >> quantityStr;
    int quantity = stoi(quantityStr);
    int stockQuantity = stoi(inventory[index][1]);

    if (quantity > stockQuantity) {
        cout << "Insufficient stock.\n";
        return;
    }

    double price = stod(inventory[index][2]);
    double totalPrice = quantity * price;

    inventory[index][1] = to_string(stockQuantity - quantity);

    sales[salesCount][0] = itemName;
    sales[salesCount][1] = to_string(quantity);
    sales[salesCount][2] = to_string(totalPrice);
    sales[salesCount][3] = to_string(time(0));
    salesCount++;

    saveInventory();
    saveSales();
    cout << "Sale recorded successfully.\n";
}

void generateReports() { // Generates a report of daily sales for the last 7 days and a total weekly sales summary.
    double dailySales[7] = { 0 };
    double totalWeeklySales = 0;
    time_t now = time(0);

    for (int i = 0; i < salesCount; i++) {
        time_t timestamp = stoi(sales[i][3]);
        if (timestamp >= now - 7 * 24 * 3600) {
            int daysAgo = (now - timestamp) / (24 * 3600);
            dailySales[daysAgo] += stod(sales[i][2]);
            totalWeeklySales += stod(sales[i][2]);
        }
    }

    cout << "\n--- Daily Sales Report ---\n";
    for (int i = 0; i < 7; i++) {
        cout << "Day " << i + 1 << ": " << dailySales[i] << "\n";
    }

    cout << "\n--- Weekly Sales Summary ---\n";
    cout << "Total Weekly Sales: " << totalWeeklySales << "\n";
}

void deleteStock() { // Deletes an item from the inventory.
    string itemName;
    cout << "Enter item name to delete: ";
    cin >> itemName;

    bool found = false;
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i][0] == itemName) {
            // Shift all items after the deleted one left by one position
            for (int j = i; j < inventoryCount - 1; j++) {
                inventory[j][0] = inventory[j + 1][0];
                inventory[j][1] = inventory[j + 1][1];
                inventory[j][2] = inventory[j + 1][2];
            }
            inventoryCount--;
            found = true;
            break;
        }
    }

    if (found) {
        saveInventory();
        cout << "Item deleted successfully.\n";
    }
    else {
        cout << "Item not found.\n";
    }
}

void searchInventory() { // Allows the admin to search for an item in the inventory.
    string itemName;
    cout << "Enter item name to search: ";
    cin >> itemName;

    bool found = false;
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i][0] == itemName) {
            cout << "\nItem Found:\n";
            cout << "Item Name: " << inventory[i][0] << endl;
            cout << "Quantity: " << inventory[i][1] << endl;
            cout << "Price: " << inventory[i][2] << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Item not found.\n";
    }
}

void viewSalesByItem() { // View total sales for a specific item.
    string itemName;
    cout << "Enter item name to view sales: ";
    cin >> itemName;

    double totalRevenue = 0;
    int totalQuantitySold = 0;
    bool found = false;

    for (int i = 0; i < salesCount; i++) {
        if (sales[i][0] == itemName) {
            totalQuantitySold += stoi(sales[i][1]);
            totalRevenue += stod(sales[i][2]);
            found = true;
        }
    }

    if (found) {
        cout << "\nSales for " << itemName << ":\n";
        cout << "Total Quantity Sold: " << totalQuantitySold << endl;
        cout << "Total Revenue: " << totalRevenue << endl;
    }
    else {
        cout << "No sales found for this item.\n";
    }
}

void adminMenu() {
    int choice;
    do {
        cout << "\nAdmin Menu:\n1. View Inventory\n2. Add Stock\n3. Record Sale\n4. Generate Reports\n5. Delete Stock\n6. Search Inventory\n7. View Sales by Item\n8. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            viewInventory();
            break;
        case 2:
            addStock();
            break;
        case 3:
            recordSale();
            break;
        case 4:
            generateReports();
            break;
        case 5:
            deleteStock();
            break;
        case 6:
            searchInventory();
            break;
        case 7:
            viewSalesByItem();
            break;
        case 8:
            cout << "Exiting Admin Menu...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 8);
}

int main() {
    loadInventory();
    loadSales();
    adminMenu();
    return 0;
}
