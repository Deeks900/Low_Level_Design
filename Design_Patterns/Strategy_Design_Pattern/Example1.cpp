#include <iostream>
using namespace std;

//Let's define Strategies First i.e Abstract classes for each behavior

class TalkableRobot {
    public:
    virtual void talk() = 0; //pure virtual function
    //virtual destructor to ensure proper cleanup of derived classes
    virtual ~TalkableRobot() {}
};

class FlyableRobot {
    public:
    virtual void fly() = 0; //pure virtual function
    virtual ~FlyableRobot() {}
};

class WalkableRobot {
    public:
    virtual void walk() = 0; //pure virtual function
    virtual ~WalkableRobot() {}
};

//Now let's define the concrete implementations of these strategies

class NormalTalkRobot : public TalkableRobot {
    public:
    void talk() override {
        cout << "I am a normal talking robot." << endl;
    }
};

class NoTalkRobot : public TalkableRobot {
    public:
    void talk() override {
        cout << "I cannot talk." << endl;
    }
};

class NormalFlyRobot : public FlyableRobot {
    public:
    void fly() override {
        cout << "I can fly normally." << endl;
    }
};

class NoFlyRobot : public FlyableRobot {
    public:
    void fly() override {
        cout << "I cannot fly." << endl;
    }
};

class NormalWalkRobot : public WalkableRobot {
    public:
    void walk() override {
        cout << "I can walk normally." << endl;
    }
};

class NoWalkRobot : public WalkableRobot {
    public:
    void walk() override {
        cout << "I cannot walk." << endl;
    }
};

//Now let's define the Robot class which will use these strategies
//We are using composition to include the behaviors in the Robot class

class Robot{
    TalkableRobot* talkBehavior;
    FlyableRobot* flyBehavior;
    WalkableRobot* walkBehavior;

    public:
    //Constructor to initialize the behaviors
    Robot(TalkableRobot* talkBehavior, FlyableRobot* flyBehavior, WalkableRobot* walkBehavior) {
        this->talkBehavior = talkBehavior;
        this->flyBehavior = flyBehavior;
        this->walkBehavior = walkBehavior;
    }

    //Destructor to clean up the behaviors
    virtual ~Robot() {
        delete talkBehavior;
        delete flyBehavior;
        delete walkBehavior;                                
    }
   
    //Now we can call function of that behavior in the Robot class
    void performTalk() {
        talkBehavior->talk();
    }

    void performFly() {
        flyBehavior->fly();
    }

    void performWalk() {
        walkBehavior->walk();
    }

    virtual void projection() = 0; // Abstract method for subclasses
};

// --- Concrete Robot Types ---
class CompanionRobot : public Robot {
public:
    CompanionRobot(WalkableRobot* w, TalkableRobot* t, FlyableRobot* f)
        : Robot(t, f, w) {}

    void projection() override {
        cout << "Displaying friendly companion features..." << endl;
    }
};

class WorkerRobot : public Robot {
public:
    WorkerRobot(WalkableRobot* w, TalkableRobot* t, FlyableRobot* f)
        : Robot(t, f, w) {}

    void projection() override {
        cout << "Displaying worker efficiency stats..." << endl;
    }
};

int main() {
    
    Robot* companion = new CompanionRobot(new NormalWalkRobot(), new NormalTalkRobot(), new NoFlyRobot());
    Robot* worker = new WorkerRobot(new NormalWalkRobot(), new NoTalkRobot(), new NormalFlyRobot());    
    
    cout << "Companion Robot:" << endl;
    companion->performTalk();
    companion->performFly();
    companion->performWalk();
    companion->projection();        
    delete companion;

    cout << "\nWorker Robot:" << endl;
    worker->performTalk();
    worker->performFly();
    worker->performWalk();  
    worker->projection();       
    delete worker;
        
    return 0;
}