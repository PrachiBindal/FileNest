#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP
#include <iostream>
#include "Task.hpp"

#include <memory>
#include <mutex>
#include <atomic>
#include <windows.h>


class ProcessManagement {
    HANDLE itemsSemaphore;
    HANDLE emptySlotsSemaphore;

public:
    ProcessManagement();
    ~ProcessManagement();
    bool submitToQueue(std::unique_ptr<Task> task);
    void executeTask();

private:
    struct SharedMemory {
        std::atomic<int> size;
        char tasks[1000][256];
        int front;
        int rear;

        void printSharedMemory() {
            std::cout << size << std::endl;
            std::cout << front << std::endl;
            std::cout << rear << std::endl;
        }
    };
    SharedMemory* sharedMem;
    HANDLE hMapFile;
    // Removed SHM_NAME from here, use local variable in constructor
    std::mutex queueLock;
};

#endif