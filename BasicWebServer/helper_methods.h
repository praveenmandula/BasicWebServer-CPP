#pragma once
#include "cpp_standard_includes.h"
#include "LogTrace/log_trace.h"

class HelperMethods {
public:
	static int saveFile(const std::string& filename, const std::string& fileData);
	static std::string getFilenameFromHeaders(const std::string& headers);
};