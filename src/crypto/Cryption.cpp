#include "../../headers/crypto/Cryption.hpp"
#include "../../headers/tasks/Task.hpp"
#include "../file/ReadEnv.cpp"
#include <ctime>
#include <iomanip>

int executeCryption(const std::string& taskData) {
    Task task = Task::fromString(taskData);
    ReadEnv env;
    std::string envKey = env.getenv();
    std::cout << "DEBUG: envKey = '" << envKey << "'" << std::endl;
    int key = 0;
    try {
        key = std::stoi(envKey);
    } catch (const std::invalid_argument&) {
        std::cerr << "Environment key is not a valid integer: " << envKey << std::endl;
        return 1;
    } catch (const std::out_of_range&) {
        std::cerr << "Environment key is out of range: " << envKey << std::endl;
        return 1;
    }
    if (task.action == Action::ENCRYPT) {
        char ch;
        while (task.f_stream.get(ch)) {
            ch = (ch + key) % 256;
            task.f_stream.seekp(-1, std::ios::cur);
            task.f_stream.put(ch);
        }
        task.f_stream.close();
    } else {
        char ch;
        while (task.f_stream.get(ch)) {
            ch = (ch - key + 256) % 256;
            task.f_stream.seekp(-1, std::ios::cur);
            task.f_stream.put(ch);
        }
        task.f_stream.close();
    }
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::cout << "Exiting the encryption/decryption at: " << std::put_time(now, "%Y-%m-%d %H:%M:%S") << std::endl;
    
    return 0;
}