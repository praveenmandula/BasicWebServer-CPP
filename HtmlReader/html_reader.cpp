#include "html_reader.h"
// HtmlReader.cpp : Defines the functions for the static library.
//

int HtmlReader::readHtmlContentFromFile(const std::string& htmlFileName, std::string& htmlFileData)
{
    std::ifstream file(htmlFileName);
    if (!file.is_open()) {
        LogMessage(LogLevel::ERROR_R, "Error opening html file: ", htmlFileName);
        return -1;
    }

#if 0
    std::string line;
    while (std::getline(file, line)) {
        htmlString += line;
    }

    file.close();
#endif

    // Read file content into a stringstream
    std::stringstream buffer;
    buffer << file.rdbuf();
    htmlFileData = buffer.str();

    return 0;
}
