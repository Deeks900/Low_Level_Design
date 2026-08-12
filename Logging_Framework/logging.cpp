#include <bits/stdc++.h>
#include <ctime>
using namespace std;

//enum class for different log levels
enum class LogLevel{
    DEBUG,
    INFO, 
    WARN,
    ERROR,
    FATAL
};

//Now we will be creating Log Message class
class LogMessage{
    private:
        LogLevel level;
        string message;
        time_t timestamp;

    public:
    //constructor
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

//Now let's move to Formatter, We will be creating a abstract class
class LogFormatter{
    public:
        virtual string formatLogMessage(LogMessage& logMessage) = 0; //pure virtual function

};

//Now we will be creating a concrete class for formatter
class defaultFormatter : public LogFormatter { 
    public:
        //Implement that pure virtual function
        string formatLogMessage(LogMessage& logMessage) { 
            //formatting of message will be done
            return logMessage.getMessage();
        }
};

//Now we will be creating a abstract class Appender 
class Appender {
    public:
        virtual void append(string formattedLogMessage) = 0;
};

//Now we will be creating concrete classes for appender 
class fileAppender : public Appender {
    public:
        void append(string logMessage){
            //here we will write the log message to a file
            return;
        }
};

class consoleAppender : public Appender {
    public:
        void append(string logMessage){
            //here we will write the log message to console
            cout << logMessage << endl;
            return;
        }
};

//Now we will be making the logger class which will be the orchestrating class
class Logger {
    private:
        string name;
        LogLevel level;
        LogFormatter* formatter;
        vector<Appender*> appenders;

        public:
            Logger(string name, LogLevel level, LogFormatter* formatter) {
            this->name = name;
            this->level = level;
            this->formatter = formatter;
            }

            void addAppender(Appender* appender){
                this->appenders.push_back(appender);
            }

            void log(LogLevel level, string message){
                if(level < this->level){
                    return; // Ignore messages below the logger's level
                }

                LogMessage logMessage = LogMessage(level, message);
                string formatLog = formatter->formatLogMessage(logMessage);
                for(auto i : appenders){
                    i->append(formatLog);
                }
            }

            //Now the functions user will be making use of to log messages
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

//Now implementing the logger Manager
class LoggerManager { 
    private:
        unordered_map<string, Logger*> loggers;
        LoggerManager() {} //For singleton pattern, private constructor

    public:
        //can only be accessed through class method, singleton pattern
        static LoggerManager& getInstance(){
            static LoggerManager instance;
            return instance;
        }

        Logger* getLogger(string name){
            if(loggers.find(name) != loggers.end()){
                return loggers[name];
            }

            //If logger with this name doesn't exist, create a new one
            Logger* newLogger = new Logger(name, LogLevel::DEBUG, new defaultFormatter());
            newLogger->addAppender(new consoleAppender());
            loggers[name] = newLogger;
            return newLogger;
        } 
};

int main(){
    LoggerManager& loggerManager = LoggerManager::getInstance();
    Logger* logger = loggerManager.getLogger("MyLogger");
    logger->info("This is an info message");
    logger->debug("This is a debug message");   
    logger->warn("This is a warning message");
    logger->error("This is an error message");
    logger->fatal("This is a fatal message");
    return 0;
}

