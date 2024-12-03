#pragma once
#include "cpp_standard_includes.h"
#include "LogTrace/log_trace.h"

class HelperMethods {
public:
	static int saveFile(std::string& fileName, std::string& fileType, std::string& fileData, std::string directory = "Uploaded_Files");
};