#include "helper_methods.h"

int HelperMethods::saveFile(std::string& fileName, std::string& fileData, std::string directory)
{
    namespace fs = std::filesystem;

    try {
        // Ensure the directory exists
        if (!fs::exists(directory)) {
            if (!fs::create_directories(directory)) {
                LogMessage(LogLevel::ERROR_R, "Failed to create directory: ", directory);
                return -1;
            }
        }

        // Construct the full file path
        fs::path filePath = fs::path(directory) / ( fileName + "test" );

        // Open the file in binary mode
        std::ofstream file(filePath, std::ios::binary | std::ios::out);
        if (!file.is_open()) {
            LogMessage(LogLevel::ERROR_R, "Failed to open file for writing, filename = ", filePath.string());
            return -1;
        }

        // Write data to the file
        file.write(fileData.c_str(), fileData.size());
        file.close();

        LogMessage(LogLevel::INFO, "File saved as ", filePath.string());
    }
    catch (const std::exception& e) {
        LogMessage(LogLevel::ERROR_R, "Error saving file: ", e.what());
        return -1;
    }
    return 0;
}

