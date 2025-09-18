# FileNest - Parallel File Encryption/Decryption System

## Overview

FileNest is a fast tool for encrypting and decrypting files on Windows. It can process multiple files at the same time using Windows processes and shared memory.

## Features

* **Parallel Processing:** Handle many files at once
* **Shared Memory:** Efficient task queue
* **Thread-Safe:** Safe access to shared resources
* **Directory Processing:** Scan and process folders automatically
* **Environment Keys:** Store encryption keys securely
* **Python Tools:** Optional setup scripts
* **High Performance:** Optimized for Windows
* **GUI**

## Requirements

* **OS:** Windows 10/11
* **Compiler:** MinGW-w64 or MSVC
* **C++17** or later
* **Python 3.7+** (for test scripts)
* **Build Tools:** Make or MSBuild

## Setup

### 1. Clone Repo

```bash
git clone <your-repo-url>
cd Encriptor
```

### 2. Set Key

Create a `.env` file with your encryption key:

```
42
```

### 3. Optional Test Setup

```bash
python -m venv myenv
venv\Scripts\activate
python setup_dirs.py
```

### 4. Build

```powershell
mingw32-make -f Buildfile
```

### 5. Run

```powershell
./main.exe
```

**Usage:**

1. Enter folder path
2. Type `encrypt` or `decrypt`
3. Files will be processed automatically

## Project Structure

```
Encriptor/
├── main.cpp
├── BuildFile
├── setup_dirs.py
├── README.md
├── src/
│   ├── tasks/ProcessManagement.cpp
│   ├── crypto/Cryption.cpp
│   ├── crypto/CryptionMain.cpp
│   └── file/IO.cpp
│       ReadEnv.cpp
├── headers/
│   ├── tasks/ProcessManagement.hpp
│   ├── tasks/Task.hpp
│   ├── crypto/Cryption.hpp
│   └── file/IO.hpp
└── data/data1.txt
```

## Key Components

* **main.cpp** - Handles user input
* **ProcessManagement** - Manages processes and memory
* **Task** - Task structure
* **Cryption** - Encrypts files
* **IO** - File input/output
* **ReadEnv** - Reads key from `.env`

## How It Works

1. **Main Process:** Scans folders and adds tasks
2. **Worker Processes:** Process files in parallel
3. **Shared Queue:** Distributes tasks safely
4. **Synchronization:** Ensures safe resource use

**Flow:**

```
Scan → Create Tasks → Shared Queue → Workers → Process Files
```

## Security

* Keys in `.env`
* Caesar cipher encryption
* Safe memory and process isolation

## Performance

* Multiple files at once
* Dynamic process creation
* Low memory use
* Windows optimized

## Notes

* Windows only
* Do not commit `.env`
* Backup important files
* Automatic cleanup included

## Commands

```powershell
mingw32-make -f Buildfile clean     #remove build
mingw32-make -f Buildfile           # Build everything
```

## Troubleshooting

* MinGW-w64 in PATH
* All headers present
* `.env` has a valid key
* Check folder access and permissions

## Future Plans

* Linux/macOS support
* AES/RSA encryption
* Logging and progress tracking
* Batch processing
* Network processing
