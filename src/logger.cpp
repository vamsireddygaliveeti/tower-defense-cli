//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "logger.h"

#include <iomanip>
#include <iostream>
#include <sstream>

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : logFile("log.txt", std::ios_base::trunc), exitFlag(false) {
    // Check if file opened successfully
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not open log file!" << std::endl;
    }
    // Start background thread for writing logs
    logThread = std::thread(&Logger::ProcessLogs, this);
}

Logger::~Logger() {
    // Signal thread to stop
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        exitFlag = true;
    }
    cv.notify_all();
    
    // Wait for thread to finish
    if (logThread.joinable()) {
        logThread.join();
    }
    
    // Close file
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::Log(const std::string& message) {
    // Add timestamp to message
    std::string fullMessage = GetCurrentTimestamp() + " " + message;
    
    // Add to queue (thread-safe)
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        logQueue.push(fullMessage);
    }
    
    // Wake up logging thread
    cv.notify_one();
}

void Logger::Flush() {
    // Wait until all messages are written
    while (true) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (logQueue.empty()) {
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    // Make sure file writes are complete
    if (logFile.is_open()) {
        logFile.flush();
    }
}

void Logger::ProcessLogs() {
    // Keep running until told to exit
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        
        // Wait for messages or exit signal
        cv.wait(lock, [this] { return !logQueue.empty() || exitFlag; });
        
        // Write all pending messages
        while (!logQueue.empty()) {
            std::string msg = logQueue.front();
            logQueue.pop();
            
            // Release lock while writing to file
            lock.unlock();
            
            if (logFile.is_open()) {
                logFile << msg << std::endl;
                logFile.flush();
            }
            
            lock.lock();
        }
        
        // Exit if flag is set and queue is empty
        if (exitFlag && logQueue.empty()) {
            break;
        }
    }
}

std::string Logger::GetCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %X");
    return ss.str();
}