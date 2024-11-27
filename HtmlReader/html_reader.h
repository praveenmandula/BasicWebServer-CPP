#pragma once
#include "cpp_standard_includes.h"

/*
 * Author: Praveen Mandula
 * Date: 2024-10-30
 */

/**
html helper class is used to read HTML files as the string, which can be used in reponse body
**/

class HtmlReader {
public:
	int readHtmlContentFromFile(const std::string& htmlFileName, std::string& htmlFileData);
};