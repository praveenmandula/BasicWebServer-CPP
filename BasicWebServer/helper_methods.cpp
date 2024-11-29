#include "helper_methods.h"

int HelperMethods::saveFile(const std::string& filename, const std::string& fileData)
{
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(fileData.c_str(), fileData.size());
        file.close();
        LogMessage(LogLevel::INFO, "File saved as ", filename);
    }
    else {
        LogMessage(LogLevel::ERROR_R, "Failed to open file for writing, filename = ", filename);
        return -1;
    }
    return 0;
}

std::string HelperMethods::getFilenameFromHeaders(const std::string& headers)
{
    // Extract filename from headers (for demonstration, we'll assume it's in a header)
    std::string filename = "uploaded_file.bin";  // Default filename
    size_t pos = headers.find("filename=");
    if (pos != std::string::npos) {
        size_t start = pos + 9; // Length of "filename="
        size_t end = headers.find("\r\n", start);
        filename = headers.substr(start, end - start);
    }
    return filename;
}
