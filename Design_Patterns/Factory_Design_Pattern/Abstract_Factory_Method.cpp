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

class GarlicBread {
    public:
    void prepare() {
        cout << "Preparing Garlic Bread." << endl;
    }
};

class BasicGarlicBread : public GarlicBread {
    public:
    void prepare() {
        cout << "Preparing Basic Garlic Bread." << endl;
    }
};

class WheatGarlicBread : public GarlicBread {
    public:
    void prepare() {
        cout << "Preparing Wheat Garlic Bread." << endl;
    }
};

class MealFactory {
    public:
    virtual Burger* createBurger(string type) = 0; //pure virtual function
    virtual GarlicBread* createGarlicBread() = 0; //pure virtual function
    virtual ~MealFactory() {}
};

class SinghBurger : public MealFactory {
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

    GarlicBread* createGarlicBread() override {
        return new BasicGarlicBread();
}
};

class KingBurger : public MealFactory {
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

    GarlicBread* createGarlicBread() override {
        return new WheatGarlicBread();
    }
};  

int main() {
    MealFactory* factory = new SinghBurger();

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
    GarlicBread* garlicBread = factory->createGarlicBread();
    if (garlicBread) {
        garlicBread->prepare();
        delete garlicBread; // Clean up                 
    }

    factory = new KingBurger(); 
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
    garlicBread = factory->createGarlicBread();
    if (garlicBread) {
        garlicBread->prepare();
        delete garlicBread; // Clean up         
    }
    delete factory; // Clean up
    return 0;
}
