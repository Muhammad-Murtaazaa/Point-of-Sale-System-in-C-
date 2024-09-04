#include <iostream>
#include <string>
#include <fstream>
using namespace std;
const int INITIAL_SIZE = 4;
const int MAX_BILL_ITEMS = 100;
class discount;
class product
{
private:
    int id, stock;
    string name;
    double price;
public:
    product(int ID = 0, string NAME = "", double PRICE = 0.0, int STOCK = 0);

    int getId() const;
    void setId(int ID);

    string getName() const;
    void setName(string NAME);

    double getPrice() const;
    void setPrice(double PRICE);

    int getStock() const;
    void setStock(int STOCK);

    friend class discount;
};

product::product(int ID, string NAME, double PRICE, int STOCK)
{
    id = ID;
    name = NAME;
    price = PRICE;
    stock = STOCK;
}

int product::getId() const
{
    return id;
}

void product::setId(int ID)
{
    id = ID;
}

string product::getName() const
{
    return name;
}

void product::setName(string NAME)
{
    name = NAME;
}

double product::getPrice() const
{
    return price;
}

void product::setPrice(double PRICE)
{
    price = PRICE;
}

int product::getStock() const
{
    return stock;
}

void product::setStock(int STOCK)
{
    stock = STOCK;
}

class discount
{
public:
    static void applyDiscount(product& obj, double discountPer)
    {
        if (discountPer < 0 || discountPer > 100)
        {
            cout << "INVALID DISCOUNT PERCENTAGE !!! ENTER A NUMBER BETWEEN 0 AND 100" << endl;
            return;
        }
        double afterdis = obj.price * (discountPer / 100);
        obj.price = obj.price - afterdis;
        cout << "APPLIED " << discountPer << "% DISCOUNT TO " << obj.name << endl << "PRICE AFTER DISCOUNT: " << obj.price << endl;
    }
};

class inventory
{
public:
    virtual void addProduct(const product& obj) = 0;
    virtual void removeProduct(int id) = 0;
    virtual void updateProduct(int id, string name, double price, int stock) = 0;
    virtual product findProductById(int id) = 0;
    virtual void displayStock() const = 0;
    virtual void saveStock() = 0;
};

class stock : public inventory
{
private:
    product* PRODUCTS;
    int size;
    int capacity;
    void resize();
public:
    stock();
    ~stock();
    void addProduct(const product& obj);
    void removeProduct(int id);
    void updateProduct(int id, string name, double price, int stock);
    product findProductById(int id);
    void displayStock() const;
    void saveStock();
    bool isUniqueId(int id);
};

stock::stock()
{
    size = 0;
    capacity = INITIAL_SIZE;
    PRODUCTS = new product[capacity];
}

stock::~stock()
{
    delete[] PRODUCTS;
}

bool stock::isUniqueId(int id)
{
    for (int i = 0; i < size; i++)
    {
        if (PRODUCTS[i].getId() == id)
        {
            return true;
        }
    }
    return false;
}
void stock::resize()
{
    capacity = capacity * 2;
    product* NEWPRODUCTS = new product[capacity];
    for (int i = 0; i < size; i++)
    {
        NEWPRODUCTS[i] = PRODUCTS[i];
    }
    delete[] PRODUCTS;
    PRODUCTS = NEWPRODUCTS;
}

void stock::addProduct(const product& obj)
{
    if (isUniqueId(obj.getId()))
    {
        cout << "ID MUST BE UNIQUE !!!!" << endl;
        return;
    }
    if (size >= capacity)
    {
        resize();
    }
    PRODUCTS[size++] = obj;
}

void stock::removeProduct(int id)
{
    for (int i = 0; i < size; i++)
    {
        if (PRODUCTS[i].getId() == id)
        {
            for (int j = i; j < size - 1; j++)
            {
                PRODUCTS[j] = PRODUCTS[j + 1];
            }
            size--;
            break;
        }
        else
        {
            cout << "NO SUCH PRODUCT FOUND !!!  :( " << endl;
        }
    }
}

void stock::updateProduct(int id, string name, double price, int stock)
{
    for (int i = 0; i < size; i++)
    {
        if (PRODUCTS[i].getId() == id)
        {
            if (!name.empty())
            {
                PRODUCTS[i].setName(name);
            }
            if (price != -1)
            {
                PRODUCTS[i].setPrice(price);
            }
            if (stock != -1)
            {
                PRODUCTS[i].setStock(stock);
            }
            break;

        }
    }
}

product stock::findProductById(int id)
{
    for (int i = 0; i < size; i++)
    {
        if (PRODUCTS[i].getId() == id)
        {
            return PRODUCTS[i];
        }
    }
    cout << "NO PRODUCT FOUND :( !!!" << endl;
}

void stock::displayStock() const
{
    if (size == 0)
    {
        cout << "NOTHING TO SHOW HERE !!!!" << endl << "ADD SOME PRODUCTS :)";
        return;
    }
    cout << "ID \t Name \t Price \t Stock" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << PRODUCTS[i].getId() << " \t " << PRODUCTS[i].getName() << " \t " << PRODUCTS[i].getPrice() << " \t " << PRODUCTS[i].getStock() << endl;
    }
}

void stock::saveStock()
{
    fstream STOCK("stock.txt",ios::out);
    for (int i = 0; i < size; ++i) 
    {
        STOCK << "ID: " << PRODUCTS[i].getId() << " , Name: " << PRODUCTS[i].getName()
            << ", Price: " << PRODUCTS[i].getPrice() << ", Stock: " << PRODUCTS[i].getStock() << endl;
    }
    STOCK.close();
    cout << "Stock saved to stock.txt ;)" << endl;
}

class billing
{
public:
    virtual void addProduct(const product& obj, int quantity) = 0;
    virtual double getTotal() const = 0;
    virtual void printBill() = 0;
    virtual void saveBill() = 0;
};

class bill : public billing
{
private:
    product purchasedProducts[MAX_BILL_ITEMS];
    int quantities[MAX_BILL_ITEMS];
    int size;
    double total, payment;

public:
    bill();
    void addProduct(const product& obj, int quantity);
    double getTotal() const;
    void printBill();
    void saveBill();
    void reset();
};

bill::bill()
{
    size = 0;
    total = 0;
}

void bill::addProduct(const product& obj, int quantity)
{
    if (size >= MAX_BILL_ITEMS)
    {
        cout << "BILL CAPACITY REACHED YOU CANT ADD MORE PRODUCTS :(" << endl;
        return;
    }
    purchasedProducts[size] = obj;
    quantities[size] = quantity;
    total = total + obj.getPrice() * quantity;
    size++;
}

double bill::getTotal() const
{
    return total;
}

void bill::printBill()
{
    cout << "=========================== YOUR RECEIPT ==============================" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << "Product: " << purchasedProducts[i].getName() << ", Quantity: " << quantities[i]
            << ", Price: " << purchasedProducts[i].getPrice() << ", Subtotal: " << purchasedProducts[i].getPrice() * quantities[i] << endl;
    }
    cout << "Total: " << total << endl;
}

void bill::saveBill()
{
    fstream file("bill.txt", ios::app);
    file << "============================= RECEIPT ==============================" << endl;
    for (int i = 0; i < size; i++)
    {
        file << "Product: " << purchasedProducts[i].getName() << ", Quantity: " << quantities[i]
            << ", Price: " << purchasedProducts[i].getPrice() << ", Subtotal: " << purchasedProducts[i].getPrice() * quantities[i] << endl;
    }
    file << "Total: " << total << endl;
    file.close();
}

void bill::reset()
{
    size = 0;
    total = 0;
}
class taxCalculator
{
public:
    virtual double calculateTax(double amount) = 0;
};

class tax : public taxCalculator
{
private:
    double taxRate;
public:
    tax(double rate = 0.1);
    double calculateTax(double amount);
};

tax::tax(double rate)
{
    taxRate = rate;
}

double tax::calculateTax(double amount)
{
    return amount * taxRate;
}

class salesRecorder
{
private:
    double totalSales;
public:
    salesRecorder();
    void recordSale(double amount);
    double getTotalSales();
};

salesRecorder::salesRecorder()
{
    totalSales = 0;
}

void salesRecorder::recordSale(double amount)
{
    totalSales = totalSales + amount;
}

double salesRecorder::getTotalSales()
{
    return totalSales;
}

class pos
{
private:
    stock STOCK;
    bill BILL;
    tax TAX;
    salesRecorder SALES;
public:
    void addProduct();
    void removeProduct();
    void updateProduct();
    void showStock() const;
    void saveStock();
    void processBill();
    void showTotalSales();
    void applyDiscount();
};

void pos::addProduct()
{
    int id;
    string name;
    double price;
    int quantity;
    system("cls");
    cout << endl << "================================================ ADD A PRODUCT ================================================" << endl;
    cout << "Enter product ID: ";
    cin >> id;
    cout << "Enter product name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter product price: ";
    cin >> price;
    cout << "Enter product quantity: ";
    cin >> quantity;
    cout << "ITEM SUCCESSFULLY ADDED :) !!!!" << endl;
    product item(id, name, price, quantity);
    STOCK.addProduct(item);
    STOCK.displayStock();
}

void pos::removeProduct()
{
    system("cls");
    STOCK.displayStock();
    cout << endl << "================================================ REMOVE A PRODUCT ================================================" << endl;
    int id;
    cout << "Enter product ID to remove: ";
    cin >> id;
    STOCK.removeProduct(id);
    cout << "PRODUCT SUCCESSFULLY REMOVED !!!!" << endl;
    STOCK.displayStock();
}

void pos::updateProduct()
{
    system("cls");
    STOCK.displayStock();
    int id;
    string name;
    double price;
    int quantity;
    cout << endl << "================================================ UPDATE A PRODUCT ================================================" << endl;
    cout << "Enter product ID to update: ";
    cin >> id;
    cout << "Enter new product name (leave blank to keep current): ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter new product price (enter -1 to keep current): ";
    cin >> price;
    cout << "Enter new product quantity (enter -1 to keep current): ";
    cin >> quantity;
    cout << "SUCCESSFULLY UPDATED :) !!!" << endl;
    STOCK.updateProduct(id, name, price, quantity);
    STOCK.displayStock();
}

void pos::showStock() const
{
    system("cls");
    cout << "======================== CURRENT STOCK ===================================" << endl;
    STOCK.displayStock();
}

void pos::saveStock()
{
    system("cls");
    cout << "SAVING STOCK ..........." << endl;
    STOCK.saveStock();
}

void pos::processBill()
{
    system("cls");
    BILL.reset();
    cout << "================================================ GENERATE BILL ================================================" << endl;
    int id, quantity;
    double totalAmount = 0;
    while (true)
    {
        STOCK.displayStock();
        cout << "Enter product ID to add to bill (-1 to finish): ";
        cin >> id;
        if (id == -1) break;
        product OBJ = STOCK.findProductById(id);
        if (OBJ.getId() == 0)
        {
            cout << "INVALID ID !!!!" << endl;
            continue;
        }
        cout << "Enter quantity: ";
        cin >> quantity;
        if (quantity > OBJ.getStock())
        {
            cout << "INSUFFICENT STOCK :(" << endl;
            continue;
        }
        OBJ.setStock(OBJ.getStock() - quantity);
        STOCK.updateProduct(OBJ.getId(), OBJ.getName(), OBJ.getPrice(), OBJ.getStock());
        BILL.addProduct(OBJ, quantity);
        SALES.recordSale(OBJ.getPrice() * quantity);
    }
    BILL.printBill();
    double taxAmount = TAX.calculateTax(BILL.getTotal());
    cout << "Tax: " << taxAmount << endl;
    cout << "Total with Tax: " << BILL.getTotal() + taxAmount << endl;
    double fullamount = BILL.getTotal() + taxAmount;
    cout << "Enter amount: ";
    double pay;
    cin >> pay;
    while (pay < fullamount)
    {
        cout << "INSUFFICIENT BALANCE!!!" << endl;
        cout << "Enter amount again: ";
        cin >> pay;
    }
    double change = pay - fullamount;
    cout << "YOUR CHANGE: " << change << endl;
    BILL.saveBill();
    cout << "================================================" << endl;
    cout << "TRANSACTION SUCCESSFULL :) !!!!" << endl;
    cout << "THANKS FOR THE PURCHASE PLEASE COME AGAIN ;) !!!!!" << endl;
    cout << "================================================" << endl;
    STOCK.displayStock();
}

void pos::showTotalSales()
{
    system("cls");
    cout << "==================================== TOTAL SALES =======================================" << endl;
    cout << "Total Sales: " << SALES.getTotalSales() << endl << endl << endl;
}

void pos::applyDiscount()
{
    system("cls");
    int id;
    double dis;
    cout << endl << "========================== APPLY DISCOUNTS ================================" << endl;
    STOCK.displayStock();
    cout << "Enter product ID to apply discount: ";
    cin >> id;
    cout << "Enter discount percentage (in numbers 0 to 100): ";
    cin >> dis;
    product item = STOCK.findProductById(id);
    if (item.getId() == 0)
    {
        cout << "INVALID ID :( !!!" << endl;
        return;
    }
    discount::applyDiscount(item, dis);
    STOCK.updateProduct(item.getId(), item.getName(), item.getPrice(), item.getStock());
}
template <class D>
class calculator {
public:
    D add(D a, D b) {
        return a + b;
    }

    D subtract(D a, D b) {
        return a - b;
    }

    D multiply(D a, D b) {
        return a * b;
    }

    D divide(D a, D b) {
        if (b == 0) {
            cout << "YOU CAN'T DIVIDE BY ZERO !!!!" << endl;
            return 0;
        }
        return a / b;
    }

    static void run() {
        D num1, num2;
        char op;
        system("cls");
        cout << "=============== CALCULATOR ================" << endl;
        cout << "ENTER FIRST NUMBER: ";
        cin >> num1;
        cout << "ENTER SECOND NUMBER: ";
        cin >> num2;
        cout << "Enter + for addition " << endl << "- for subtraction" << endl << "* for multiplication" << endl << "/ for division" << endl;
        cout << "Enter your option:";
        cin >> op;
        calculator<D> calc;
        switch (op) {
        case '+':
            cout << "ADDITION: " << calc.add(num1, num2) << endl;
            break;
        case '-':
            cout << "SUBTRACTION: " << calc.subtract(num1, num2) << endl;
            break;
        case '*':
            cout << "MULTIPLICATION: " << calc.multiply(num1, num2) << endl;
            break;
        case '/':
            cout << "DIVISION: " << calc.divide(num1, num2) << endl;
            break;
        default:
            cout << "Enter a valid operation" << endl;
            break;
        }
    }
};
class menu
{
public:
    void showMainMenu(pos& obj);
private:
    void manageProducts(pos& obj);
};

void menu::showMainMenu(pos& obj) {
    char choice;
    do {
        cout << "--------------------------------------- POS SYETEM ---------------------------------------" << endl;
        cout << "1) Manage Products" << endl;
        cout << "2) Generate Bill" << endl;
        cout << "3) Show Stock" << endl;
        cout << "4) Show Total Sales" << endl;
        cout << "5) Apply Discount" << endl;
        cout << "6) Save Stock" << endl;
        cout << "7) Calculator" << endl;
        cout << "0) Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
        case '1':
            system("cls");
            manageProducts(obj);
            break;
        case '2':
            system("cls");
            obj.processBill();
            break;
        case '3':
            system("cls");
            obj.showStock();
            break;
        case '4':
            system("cls");
            obj.showTotalSales();
            break;
        case '5':
            system("cls");
            obj.applyDiscount();
            break;
        case '6':
            system("cls");
            obj.saveStock();
            break;
        case '7':
            system("cls");
            calculator<double>::run();
            break;
        case '0':
            system("cls");
            cout << "BYE BYE ..........." << endl;
            break;
        default:
            system("cls");
            cout << "INVALID CHOICE :(  , TRY AGAINNN !!!!" << endl;
            break;
        }
    } while (choice != '0');
}

void menu::manageProducts(pos& obj) {
    char option;
    do {
        cout << endl << "--------------------------------------- MANAGE PRODUCTS ---------------------------------------" << endl;
        cout << "1) Add Product" << endl;
        cout << "2) Remove Product" << endl;
        cout << "3) Update Product" << endl;
        cout << "0) Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> option;
        switch (option)
        {
        case '1':
            system("cls");
            obj.addProduct();
            break;
        case '2':
            system("cls");
            obj.removeProduct();
            break;
        case '3':
            system("cls");
            obj.updateProduct();
            break;
        case '0':
            system("cls");
            break;
        default:
            system("cls");
            cout << "INVALID CHOICE :( TRY AGAINNN !!!" << endl;
            break;
        }
    } while (option != '0');
}

int main()
{
    pos run;
    menu start;
    start.showMainMenu(run);
    return 0;
}

