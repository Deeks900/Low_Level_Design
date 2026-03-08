#include <iostream>
using namespace std;

class Burger {
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

class WheatVegBurger : public Burger {
    public:
    void prepare() override {
        cout << "Preparing Wheat Veg Burger." << endl;
    }
};

class WheatChickenBurger : public Burger {
    public:
    void prepare() override {
        cout << "Preparing Wheat Chicken Burger." << endl;
    }
};

class WheatOverloadCheeseBurger : public Burger {
    public:
    void prepare() override {
        cout << "Preparing Wheat Overload Cheese Burger." << endl;
    }
};

class BurgerFactory {
    public:
    virtual Burger* createBurger(string type) = 0; //pure virtual function
    virtual ~BurgerFactory() {}
};

class SimpleBurgerFactory : public BurgerFactory {
    public:
    Burger* createBurger(string type) override {
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

class WheatBurgerFactory : public BurgerFactory {
    public:
    Burger* createBurger(string type) override {
        Burger* result = nullptr;

        if (type == "Veg") {
            result = new WheatVegBurger();
        } else if (type == "Chicken") {
            result = new WheatChickenBurger();
        } else if (type == "OverloadCheese") {
            result = new WheatOverloadCheeseBurger();
        } else {
            cout << "Invalid burger type." << endl;
        }
        return result;
    }

};

int main() {
    BurgerFactory* factory = new SimpleBurgerFactory();

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

    Burger* overloadCheeseBurger = factory->createBurger("OverloadCheese");
    if (overloadCheeseBurger) {
        overloadCheeseBurger->prepare();
        delete overloadCheeseBurger; // Clean up
    }

    delete factory; // Clean up factory

    cout << "Now using Wheat Burger Factory:" << endl;

    factory = new WheatBurgerFactory();

    vegBurger = factory->createBurger("Veg");
    if (vegBurger) {
        vegBurger->prepare();
        delete vegBurger; // Clean up
    }

    chickenBurger = factory->createBurger("Chicken");
    if (chickenBurger) {
        chickenBurger->prepare();
        delete chickenBurger; // Clean up
    }

    overloadCheeseBurger = factory->createBurger("OverloadCheese");
    if (overloadCheeseBurger) {
        overloadCheeseBurger->prepare();
        delete overloadCheeseBurger; // Clean up
    }

    delete factory; // Clean up factory
    return 0;
}