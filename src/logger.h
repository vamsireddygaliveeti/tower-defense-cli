//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

// Logger class - writes game messages to log file
// Using separate thread so logging doesn't slow down the game
// Format: "2024-08-30 14:48:16 Starting game..."
class Logger {
   public:
    static Logger& GetInstance();

    ~Logger();

    // Add a message to the log
    void Log(const std::string& message);
    
    // Make sure all messages are written before exiting
    void Flush();

   private:
    Logger();
    
    // Make sure we can't copy the logger
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream logFile;
    std::thread logThread;
    std::queue<std::string> logQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool exitFlag;

    void ProcessLogs();
    std::string GetCurrentTimestamp();
};

#endif  // LOGGER_H