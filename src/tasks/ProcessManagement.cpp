
#include <iostream>
#include "../../headers/processes/ProcessManagement.hpp"
#include <windows.h>
#include <cstring>
#include "../../headers/encryptDecrypt/Cryption.hpp"
#include <atomic>


ProcessManagement::ProcessManagement() {
    itemsSemaphore = CreateSemaphoreW(NULL, 0, 1000, L"Global\\ItemsSemaphore");
    emptySlotsSemaphore = CreateSemaphoreW(NULL, 1000, 1000, L"Global\\EmptySlotsSemaphore");
    LPCWSTR shmName = L"Local\\MyQueue";
    hMapFile = CreateFileMappingW(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        sizeof(SharedMemory),
        shmName
    );
    DWORD lastErr = GetLastError();
    if (hMapFile == NULL) {
        std::cerr << "Could not create file mapping object (" << lastErr << ")\n";
        exit(1);
    }
    sharedMem = (SharedMemory*) MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        sizeof(SharedMemory)
    );
    lastErr = GetLastError();
    if (sharedMem == NULL) {
        std::cerr << "Could not map view of file (" << lastErr << ")\n";
        CloseHandle(hMapFile);
        exit(1);
    }
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}


ProcessManagement::~ProcessManagement() {
    UnmapViewOfFile(sharedMem);
    CloseHandle(hMapFile);
    CloseHandle(itemsSemaphore);
    CloseHandle(emptySlotsSemaphore);
}


bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    WaitForSingleObject(emptySlotsSemaphore, INFINITE);
    std::unique_lock<std::mutex> lock(queueLock);

    if (sharedMem->size.load() >= 1000) {
        ReleaseSemaphore(emptySlotsSemaphore, 1, NULL);
        return false;
    }
    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);
    lock.unlock();
    ReleaseSemaphore(itemsSemaphore, 1, NULL);

    // Create a new process to execute the task
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Prepare command line: CryptionMain.exe "taskString"
    std::wstring exePath = L"cryption.exe ";
    std::string taskStr = task->toString();
    std::wstring wTaskStr(taskStr.begin(), taskStr.end());
    std::wstring cmdLine = exePath + L"\"" + wTaskStr + L"\"";

    if (!CreateProcessW(NULL, &cmdLine[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")\n";
        return false;
    }
    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}


void ProcessManagement::executeTask() {
    WaitForSingleObject(itemsSemaphore, INFINITE);
    std::unique_lock<std::mutex> lock(queueLock);
    char taskStr[256];
    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);
    lock.unlock();
    ReleaseSemaphore(emptySlotsSemaphore, 1, NULL);

    executeCryption(taskStr);
}