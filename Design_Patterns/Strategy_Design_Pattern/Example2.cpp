#include <iostream>
using namespace std;

//Let's define the strategies first i.e Abstract classes for each behavior
class PaymentStrategy {
    public:
    virtual void pay(int amount) = 0; //pure virtual function
    virtual ~PaymentStrategy() {}
};

//let's define the concrete implementations of these strategies
class CreditCardPayment : public PaymentStrategy {
    string cardNumber;
    string cardHolderName;
    string expiryDate;

    public:
    CreditCardPayment(string cardNumber, string cardHolderName, string expiryDate) {
        this->cardNumber = cardNumber;
        this->cardHolderName = cardHolderName;
        this->expiryDate = expiryDate;
    }

    void pay(int amount) override {
        cout << "Paid " << amount << " using Credit Card." << endl;
    }
};

class PayPalPayment : public PaymentStrategy {
    string email;

    public:
    PayPalPayment(string email) {
        this->email = email;
    }

    void pay(int amount) override {
        cout << "Paid " << amount << " using PayPal." << endl;
    }
};

class UPI_Payment : public PaymentStrategy {
    string upiId;

    public:
    UPI_Payment(string upiId) {
        this->upiId = upiId;
    }

    void pay(int amount) override {
        cout << "Paid " << amount << " using UPI." << endl;
    }
};

//Now let's define the payment context which will use these strategies
class PaymentContext {
    PaymentStrategy* paymentStrategy;

    public:
    PaymentContext(PaymentStrategy* paymentStrategy) {
        this->paymentStrategy = paymentStrategy;
    }

    void pay(int amount) {
        paymentStrategy->pay(amount);
    }    
};

int main() {
    int choice;
    double amount;
    
    cout << "Enter amount to pay: ";
    cin >> amount;

    cout << "Choose payment method:\n";
    cout << "1. Credit Card\n2. UPI\n3. PayPal\n";
    cin >> choice;

    PaymentStrategy* paymentStrategy;
    switch (choice) {
        case 1:
            paymentStrategy = new CreditCardPayment("1234-5678-9012-3456", "John Doe", "12/25");
            break;
        case 2:
            paymentStrategy = new UPI_Payment("john@upi");
            break;
        case 3:
            paymentStrategy = new PayPalPayment("john@example.com");
            break;
        default:
            cout << "Invalid choice!" << endl;
            return 1;
    }

    PaymentContext* context = new PaymentContext(paymentStrategy);
    context->pay(amount);

    delete paymentStrategy; // Clean up memory
    delete context;
}