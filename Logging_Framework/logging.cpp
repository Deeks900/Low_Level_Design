#include <bits/stdc++.h>
#include <ctime>
#include <mutex>
#include <thread>
#include <memory>

using namespace std;

// enum class for different log levels
enum class LogLevel{
    DEBUG,
    INFO, 
    WARN,
    ERROR,
    FATAL
};

// Now we will be creating Log Message class
// Single Responsibility Principle satisfied here, as this class is only responsible for creating log messages
class LogMessage{
    private:
        LogLevel level;
        string message;
        time_t timestamp;

    public:
        // constructor
        LogMessage(LogLevel level, string message) {
            this->level = level;
            this->message = message;
            this->timestamp = time(nullptr);
        }  

        LogLevel getLogLevel() {
            return level;
        }           

        string getMessage() {
            return message;
        }   

        time_t getTimestamp() {
            return timestamp;
        }
};

// Now let's move to Formatter, We will be creating an abstract class
class LogFormatter{
    public:
        virtual ~LogFormatter() {}
        virtual string formatLogMessage(LogMessage& logMessage) = 0;
};

// Now we will be creating a concrete class for formatter
class defaultFormatter : public LogFormatter { 
    public:
        // Implement that pure virtual function
        string formatLogMessage(LogMessage& logMessage) override { 
            // formatting of message will be done
            return logMessage.getMessage();
        }
};

// Now we will be creating an abstract class Appender 
class Appender {
    public:
        virtual ~Appender(){}
        virtual void append(string formattedLogMessage) = 0;
};

// Now we will be creating concrete classes for appender 
class fileAppender : public Appender {
    private:
        mutex mtx; // Mutex for thread safety

    public:
        void append(string logMessage) override {
            lock_guard<mutex> lock(mtx);

            // here we will write the log message to a file
            return;
        }
};

class consoleAppender : public Appender {
    private:
        mutex mtx; // Mutex for thread safety

    public:
        void append(string logMessage) override {

            lock_guard<mutex> lock(mtx);

            // here we will write the log message to console
            cout << logMessage << endl;
        }
};

// Now we will be making the logger class which will be the orchestrating class
class Logger {
    private:
        string name;
        LogLevel level;

        // shared_ptr instead of raw pointer
        shared_ptr<LogFormatter> formatter;

        // shared_ptr instead of raw pointers
        vector<shared_ptr<Appender>> appenders;

        // Protects the appenders vector
        mutex appendersMutex;

    public:
        Logger(
            string name,
            LogLevel level,
            shared_ptr<LogFormatter> formatter
        ) {
            this->name = name;
            this->level = level;
            this->formatter = formatter;
        }

        // No destructor needed for formatter or appenders.
        // shared_ptr automatically manages their lifetime.

        void addAppender(shared_ptr<Appender> appender){

            lock_guard<mutex> lock(appendersMutex);

            this->appenders.push_back(appender);
        }

        void log(LogLevel level, string message){

            if(level < this->level){
                return; // Ignore messages below logger's level
            }

            LogMessage logMessage(level, message);

            string formatLog =
                formatter->formatLogMessage(logMessage);

            // Take a snapshot of the appenders
            vector<shared_ptr<Appender>> currentAppenders;

            {
                lock_guard<mutex> lock(appendersMutex);

                currentAppenders = appenders;
            } // appendersMutex released here

            // No Logger vector lock now
            for(auto appender : currentAppenders){
                appender->append(formatLog);
            }
        }

        // Now the functions user will be making use of to log messages
        void debug(string message){
            log(LogLevel::DEBUG, message);
        }

        void info(string message){
            log(LogLevel::INFO, message);
        }

        void warn(string message){
            log(LogLevel::WARN, message);
        }

        void error(string message){
            log(LogLevel::ERROR, message);
        }

        void fatal(string message){
            log(LogLevel::FATAL, message);
        }
};

// Now implementing the logger Manager
class LoggerManager { 
    private:
        unordered_map<string, Logger*> loggers;

        LoggerManager() {} // For singleton pattern, private constructor

    public:
        // Can only be accessed through class method, singleton pattern
        static LoggerManager& getInstance(){
            static LoggerManager instance;
            return instance;
        }

        // destructor
        ~LoggerManager() {
            for(auto i: loggers){
                delete i.second;
            }
        }

        Logger* getLogger(string name){

            if(loggers.find(name) != loggers.end()){
                return loggers[name];
            }

            // If logger with this name doesn't exist, create a new one

            shared_ptr<LogFormatter> formatter =
                make_shared<defaultFormatter>();

            Logger* newLogger =
                new Logger(
                    name,
                    LogLevel::DEBUG,
                    formatter
                );

            shared_ptr<Appender> console =
                make_shared<consoleAppender>();

            newLogger->addAppender(console);

            loggers[name] = newLogger;

            return newLogger;
        } 
};

int main(){

    LoggerManager& loggerManager =
        LoggerManager::getInstance();

    Logger* logger =
        loggerManager.getLogger("MyLogger");

    logger->info("This is an info message");

    logger->debug("This is a debug message");   

    logger->warn("This is a warning message");

    logger->error("This is an error message");

    logger->fatal("This is a fatal message");

    return 0;
}

