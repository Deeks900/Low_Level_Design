#include <iostream>
using namespace std;

class Burger{
    public:
    virtual void prepare() = 0; //pure virtual function
    virtual ~Burger() {}
};

class VegBurger : public Burger {
    public:
    void prepare() override {
        cout << "Preparing Veg Burger." << endl;
    }
};

class ChickenBurger : public Burger {
    public:
    void prepare() override {
        cout << "Preparing Chicken Burger." << endl;
    }
};

class OverloadCheeseBurger : public Burger {
    public:
    void prepare() override {
        cout << "Preparing Overload Cheese Burger." << endl;
    }
};

class BurgerFactory {

    public:
    Burger* createBurger(string type) {
        Burger* result = nullptr;

        if (type == "Veg") {
            result = new VegBurger();
        } else if (type == "Chicken") {
            result = new ChickenBurger();
        } else if (type == "OverloadCheese") {
            result = new OverloadCheeseBurger();
        } else {
            cout << "Invalid burger type." << endl;
        }
        return result;
    }
};

int main() {
    BurgerFactory* factory = new BurgerFactory();

    Burger* vegBurger = factory->createBurger("Veg");
    if (vegBurger) {
        vegBurger->prepare();
        delete vegBurger; // Clean up
    }

    Burger* chickenBurger = factory->createBurger("Chicken");
    if (chickenBurger) {
        chickenBurger->prepare();
        delete chickenBurger; // Clean up
    }

    Burger* cheeseBurger = factory->createBurger("OverloadCheese");
    if (cheeseBurger) {
        cheeseBurger->prepare();
        delete cheeseBurger; // Clean up
    }

    Burger* invalidBurger = factory->createBurger("Fish");
    if (invalidBurger) {
        invalidBurger->prepare();
        delete invalidBurger; // Clean up
    }
    return 0;
}