#include <bits/stdc++.h>
#include <ctime>
#include <mutex>
#include <thread>
#include <memory>
#include <condition_variable>

using namespace std;


// ============================================================
// LOG LEVEL
// ============================================================

enum class LogLevel{
    DEBUG,
    INFO, 
    WARN,
    ERROR,
    FATAL
};


// ============================================================
// LOG MESSAGE
// ============================================================

class LogMessage{
    private:
        LogLevel level;
        string message;
        time_t timestamp;

    public:

        LogMessage(LogLevel level, string message) {
            this->level = level;
            this->message = message;
            this->timestamp = time(nullptr);
        }  

        LogLevel getLogLevel() const {
            return level;
        }           

        string getMessage() const {
            return message;
        }   

        time_t getTimestamp() const {
            return timestamp;
        }
};


// ============================================================
// FORMATTER
// ============================================================

class LogFormatter{
    public:

        virtual ~LogFormatter() {}

        virtual string formatLogMessage(const LogMessage& logMessage) = 0;
};


// ============================================================
// DEFAULT FORMATTER
// ============================================================

class defaultFormatter : public LogFormatter { 
    public:

        string formatLogMessage(const LogMessage& logMessage) override { 
            return logMessage.getMessage();
        }
};


// ============================================================
// APPENDER
// ============================================================

class Appender {
    protected:
        shared_ptr<LogFormatter> formatter;

    public:

        Appender(shared_ptr<LogFormatter> formatter)
            : formatter(formatter) {}

        virtual ~Appender(){}

        virtual void append(const LogMessage& logMessage) = 0;
};


// ============================================================
// FILE APPENDER
// ============================================================

class fileAppender : public Appender {
    private:

        mutex mtx;

    public:

        fileAppender(shared_ptr<LogFormatter> formatter)
            : Appender(formatter) {}

        void append(const LogMessage& logMessage) override {

            lock_guard<mutex> lock(mtx);

            string formattedLogMessage =
                formatter->formatLogMessage(logMessage);

            // Write formattedLogMessage to file

            return;
        }
};


// ============================================================
// CONSOLE APPENDER
// ============================================================

class consoleAppender : public Appender {
    private:

        mutex mtx;

    public:

        consoleAppender(shared_ptr<LogFormatter> formatter)
            : Appender(formatter) {}

        void append(const LogMessage& logMessage) override {

            lock_guard<mutex> lock(mtx);

            string formattedLogMessage =
                formatter->formatLogMessage(logMessage);

            cout << formattedLogMessage << endl;
        }
};


// ============================================================
// LOGGER
// ============================================================

class Logger {

    private:

        string name;
        LogLevel level;


        // ----------------------------------------------------
        // APPENDERS
        // ----------------------------------------------------

        vector<shared_ptr<Appender>> appenders;

        mutex appendersMutex;


        // ----------------------------------------------------
        // LOG QUEUE
        // ----------------------------------------------------

        queue<LogMessage> logQueue;

        mutex queueMutex;

        condition_variable cv;


        // ----------------------------------------------------
        // WORKER THREAD
        // ----------------------------------------------------

        thread workerThread;


        // ----------------------------------------------------
        // SHUTDOWN FLAG
        // ----------------------------------------------------

        bool running = true;


        // ====================================================
        // WORKER FUNCTION
        // ====================================================

        void processLogs(){

            while(true){

                LogMessage message(LogLevel::INFO, "");


                // ------------------------------------------------
                // Take one message from queue
                // ------------------------------------------------

                {
                    unique_lock<mutex> lock(queueMutex);


                    /*
                        Worker sleeps until:

                        1. Queue has a log
                                   OR
                        2. Logger is shutting down
                    */

                    cv.wait(lock, [this] {

                        return !logQueue.empty() || !running;

                    });


                    /*
                        If Logger is shutting down
                        AND there are no logs remaining,
                        worker can exit.
                    */

                    if(!running && logQueue.empty()){

                        break;
                    }


                    // Take oldest log
                    message = logQueue.front();

                    logQueue.pop();

                } // queueMutex released here


                // ------------------------------------------------
                // Take snapshot of appenders
                // ------------------------------------------------

                vector<shared_ptr<Appender>> currentAppenders;

                {
                    lock_guard<mutex> lock(appendersMutex);

                    currentAppenders = appenders;

                } // appendersMutex released here


                // ------------------------------------------------
                // Send log to appenders
                // ------------------------------------------------

                for(auto appender : currentAppenders){

                    appender->append(message);
                }
            }
        }


    public:


        // ====================================================
        // CONSTRUCTOR
        // ====================================================

        Logger(
            string name,
            LogLevel level
        ){

            this->name = name;
            this->level = level;


            // Start worker thread
            workerThread = thread(
                &Logger::processLogs,
                this
            );
        }


        // ====================================================
        // DESTRUCTOR
        // ====================================================

        ~Logger(){

            /*
                Tell worker that Logger is shutting down.
            */

            {
                lock_guard<mutex> lock(queueMutex);

                running = false;
            }


            /*
                Worker may currently be sleeping
                on condition_variable.

                Wake it up.
            */

            cv.notify_one();


            /*
                Wait for worker to finish
                processing remaining logs.
            */

            workerThread.join();
        }


        // ====================================================
        // ADD APPENDER
        // ====================================================

        void addAppender(shared_ptr<Appender> appender){

            lock_guard<mutex> lock(appendersMutex);

            appenders.push_back(appender);
        }


        // ====================================================
        // LOG
        // ====================================================

        void log(LogLevel level, string message){

            if(level < this->level){

                return;
            }


            // ------------------------------------------------
            // Create LogMessage
            // ------------------------------------------------

            LogMessage logMessage(level, message);


            // ------------------------------------------------
            // Put message into queue
            // ------------------------------------------------

            {
                lock_guard<mutex> lock(queueMutex);


                /*
                    Logger may have started shutting down.

                    Don't accept new logs after that.
                */

                if(!running){

                    return;
                }


                logQueue.push(logMessage);

            } // queueMutex released here


            // ------------------------------------------------
            // Wake worker
            // ------------------------------------------------

            cv.notify_one();
        }


        // ====================================================
        // USER FACING FUNCTIONS
        // ====================================================

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


// ============================================================
// LOGGER MANAGER
// ============================================================

class LoggerManager { 

    private:

        unordered_map<string, Logger*> loggers;


        // Private constructor
        LoggerManager() {}


    public:


        // ====================================================
        // SINGLETON
        // ====================================================

        static LoggerManager& getInstance(){

            static LoggerManager instance;

            return instance;
        }


        // ====================================================
        // DESTRUCTOR
        // ====================================================

        ~LoggerManager(){

            for(auto i : loggers){

                delete i.second;
            }
        }


        // ====================================================
        // GET LOGGER
        // ====================================================

        Logger* getLogger(string name){

            if(loggers.find(name) != loggers.end()){

                return loggers[name];
            }


            // ------------------------------------------------
            // Create Logger
            // ------------------------------------------------

            Logger* newLogger =
                new Logger(
                    name,
                    LogLevel::DEBUG
                );


            // ------------------------------------------------
            // Create formatter for console
            // ------------------------------------------------

            shared_ptr<LogFormatter> formatter =
                make_shared<defaultFormatter>();


            // ------------------------------------------------
            // Create console appender with formatter
            // ------------------------------------------------

            shared_ptr<Appender> console =
                make_shared<consoleAppender>(formatter);


            newLogger->addAppender(console);


            // ------------------------------------------------
            // Store Logger
            // ------------------------------------------------

            loggers[name] = newLogger;


            return newLogger;
        } 
};


// ============================================================
// MAIN
// ============================================================

int main(){

    LoggerManager& loggerManager =
        LoggerManager::getInstance();


    Logger* logger =
        loggerManager.getLogger("MyLogger");


    logger->info(
        "This is an info message"
    );

    logger->debug(
        "This is a debug message"
    );   

    logger->warn(
        "This is a warning message"
    );

    logger->error(
        "This is an error message"
    );

    logger->fatal(
        "This is a fatal message"
    );


    /*
        DO NOT:

        delete logger;
        delete &loggerManager;

        LoggerManager owns the Logger.
        LoggerManager's destructor will delete it.

        And Logger's destructor will:
            1. stop accepting new logs
            2. wake worker
            3. process remaining logs
            4. join worker
    */

    return 0;
}