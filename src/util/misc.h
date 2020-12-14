#ifndef SRC_UTIL_MISC_H_
#define SRC_UTIL_MISC_H_

#include <string>
#include <vector>

namespace util {
std::string toUtf8(const std::u32string& s);

std::u32string toUtf32(const std::string& s);

std::u32string unescape(std::u32string &s);

bool listFiles(std::string dir, std::vector<std::string> &files);

bool checkSubDir(std::string dir, std::string subDir);

bool checkFileInDir(std::string dir, std::string fileName);

void splitFileName(std::string &fullName, std::string &fileName, std::string &extName);

bool isIdentifier(std::string &s);

std::string toLowerCase(std::string &s);
}

#endif
