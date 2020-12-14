#include <locale>
#include <codecvt>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "misc.h"

using namespace std;

namespace util {
std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;

std::string toUtf8(const std::u32string& s) {
	return conv.to_bytes(s);
}

std::u32string toUtf32(const std::string& s) {
	return conv.from_bytes(s);
}

static char32_t quickParseHex(u32string &s, int off, int len) {
	char32_t result = 0;
	for (int i = 0; i < len; i++) {
		int c = s[off + i];

		if (c >= 'a') {
			c -= 'a';
			c += 10;
		} else if (c >= 'A') {
			c -= 'A';
			c += 10;
		} else {
			c -= '0';
		}

		result *= 16;
		result += c;
	}
	return result;
}

u32string unescape(u32string &s) {
	int len = s.length() - 1; // skip last "
	char32_t chars[len];
	int pos = 1;
	int i = 0;
	while (pos < len) {
		char32_t ch = s[pos];
		pos++;
		if (ch != '\\') {
			chars[i++] = ch;
		} else {
			ch = s[pos];
			pos++;
			switch (ch) {
			case 't':
				chars[i++] = '\t';
				continue;
			case 'n':
				chars[i++] = '\n';
				continue;
			case 'r':
				chars[i++] = '\r';
				continue;
			case '"':
				chars[i++] = '\"';
				continue;
			case '\\':
				chars[i++] = '\\';
				continue;
			}
			if (ch == 'u') { // unicode escape
				ch = (char32_t) quickParseHex(s, pos, 4);
				chars[i++] = ch;
				pos += 4;
			} else { // 'U'
				ch = (char32_t) quickParseHex(s, pos, 6);
				chars[i++] = ch;
				pos += 6;
			}
		}
	}
	return u32string(chars, i);
}

// implementation on Posix, don't support Chinese characters
bool listFiles(string dir, vector<string> &files) {
	DIR *dp;
	struct dirent *dirp;
	struct stat st;

	if ((dp = opendir(dir.c_str())) == NULL) {
		return false;
	}

	while ((dirp = readdir(dp)) != NULL) {
		string fname = dir + "/" + dirp->d_name;
		if (!stat(fname.c_str(), &st)) {
			if ((st.st_mode & S_IFREG) != 0) {
				files.push_back(string(dirp->d_name));
			}
		}
	}

	closedir(dp);
	return true;
}

bool checkSubDir(std::string dir, std::string subDir) {
	struct stat st;

	string path = dir + "/" + subDir;
	if (!stat(path.c_str(), &st)) {
		if ((st.st_mode & S_IFDIR) != 0) {
			return true;
		}
	}
	return false;
}

bool checkFileInDir(std::string dir, std::string fileName) {
	struct stat st;

	string path = dir + "/" + fileName;
	if (!stat(path.c_str(), &st)) {
		if ((st.st_mode & S_IFREG) != 0) {
			return true;
		}
	}
	return false;
}

void splitFileName(std::string &fullName, std::string &fileName,
		std::string &extName) {
	size_t pos = fullName.rfind('.');
	if (pos == string::npos) {
		fileName = fullName;
		extName = "";
	} else {
		fileName = fullName.substr(0, pos);
		extName = fullName.substr(pos + 1, fullName.length() - pos - 1);
	}
}

static bool isIdentStart(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isIdentifier(std::string &s) {
	if (s.length() == 0) {
		return false;
	}

	if (!isIdentStart(s[0])) {
		return false;
	}
	for (size_t i = 1; i < s.length(); i++) {
		if (!isIdentStart(s[i]) && !(s[i] >= '0' && s[i] <= '9')) {
			return false;
		}
	}
	return true;
}

std::string toLowerCase(std::string &s) {
	string low = s;
	for (size_t i = 0; i < low.length(); i++) {
		if (isupper(low[i])) {
			low[i] = tolower(low[i]);
		}
	}
	return low;
}

}
