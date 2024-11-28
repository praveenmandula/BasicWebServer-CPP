/*
 * Author: Praveen Mandula
 * Date: 11/02/2024
 * Description : HtmlReader class is used to read html content from the file and returns as a std::string
 */
#include "html_reader.h"
int HtmlReader::readHtmlContentFromFile(const std::string& htmlFileName, std::string& htmlFileData)
{
    std::ifstream file(htmlFileName);
    if (!file.is_open()) {
        LogMessage(LogLevel::ERROR_R, "Error opening html file: ", htmlFileName);
        return -1;
    }

    // Read file content into a stringstream
    std::stringstream buffer;
    buffer << file.rdbuf();
    htmlFileData = buffer.str();

    return 0;
}
