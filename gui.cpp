#include <shlobj.h>   // ✅ add this
#include <windows.h>
#include <commdlg.h>
#include <string>
#include "./headers/tasks/ProcessManagement.hpp"
#include "./headers/file/IO.hpp"
#include "./headers/tasks/Task.hpp"
#include <filesystem>
#include <fstream>
#include <ctime>
#include <iomanip>

namespace fs = std::filesystem;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    // Choose directory with a dialog
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = "Select a folder to encrypt/decrypt:";
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    if (!pidl) return 0;

    char path[MAX_PATH];
    if (!SHGetPathFromIDList(pidl, path)) return 0;

    std::string directory(path);

    // Ask for action with a simple MessageBox
    int choice = MessageBox(NULL, "Encrypt? (Yes = Encrypt, No = Decrypt)", "Choose Action", MB_YESNO | MB_ICONQUESTION);
    std::string action = (choice == IDYES) ? "encrypt" : "decrypt";

    try {
        if (fs::exists(directory) && fs::is_directory(directory)) {
            ProcessManagement processManagement;

            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    IO io(filePath);
                    std::fstream f_stream = std::move(io.getFileStream());

                    if (f_stream.is_open()) {
                        Action taskAction = (action == "encrypt") ? Action::ENCRYPT : Action::DECRYPT;
                        auto task = std::make_unique<Task>(std::move(f_stream), taskAction, filePath);
                        processManagement.submitToQueue(std::move(task));
                    }
                }
            }

            MessageBox(NULL, "Operation Completed!", "Success", MB_OK | MB_ICONINFORMATION);
        } else {
            MessageBox(NULL, "Invalid Directory!", "Error", MB_OK | MB_ICONERROR);
        }
    } catch (const fs::filesystem_error& ex) {
        MessageBox(NULL, ex.what(), "Filesystem Error", MB_OK | MB_ICONERROR);
    }

    return 0;
}
