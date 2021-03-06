#include "rdi_wfileio.h"
#include "utf8_file_dir.h"

#include <sstream>
#include <fstream>
#include <codecvt>
#include <utility>
#include <assert.h>
#include <algorithm>
#include <memory>

using namespace std;

namespace RDI
{

wstring read_wfile(const string& filename)
{
	wifstream wif(filename.c_str());
	wif.imbue(locale(wif.getloc(), new codecvt_utf8<wchar_t>));
	wstringstream wss;
	wss << wif.rdbuf();
	return wss.str();
}

bool write_wfile(const string& filename, const wstring& fileContent)
{
	wofstream wof(filename.c_str());
	if(!wof.is_open())
		return false;
	wof.imbue(locale(wof.getloc(), new codecvt_utf8<wchar_t>));
	wof << fileContent;
	return true;
}

string read_file(const string &filename)
{
	ifstream stream(filename.c_str());
	stringstream ss;
	ss << stream.rdbuf();
	return ss.str();
}

bool write_file(const string &filename, const string &fileContent)
{
	ofstream of(filename.c_str());
	if(!of.is_open())
		return false;
	of << fileContent;
	return true;
}

string get_current_directory()
{
	char* strPath = new char[512];
	CubicleSoft::UTF8::Dir::Getcwd(strPath, 512);

	string path(strPath);
	delete[] strPath;
	return path + "/";
}

vector<wstring> read_wfile_lines(const string &filename)
{
	wifstream wif(filename.c_str());
	wif.imbue(locale(wif.getloc(), new codecvt_utf8<wchar_t>));
	wstringstream wss;
	wss << wif.rdbuf();

	wstring tmp;
	vector<wstring> lines;
	while (getline(wss, tmp))
		lines.push_back(tmp);

	return lines;
}

bool write_wfile_lines(const string &filename, const vector<wstring> &linesToWrite)
{
	wofstream wof(filename.c_str());
	if(!wof.is_open())
		return false;
	wof.imbue(locale(wof.getloc(), new codecvt_utf8<wchar_t>));

	for (wstring line : linesToWrite)
		wof << line << endl;

	return true;
}

vector<string> read_file_lines(const string &filename)
{
	ifstream stream(filename.c_str());
	stringstream ss;
	ss << stream.rdbuf();

	string tmp;
	vector<string> lines;
	while (getline(ss, tmp))
		lines.push_back(tmp);

	return lines;
}

bool write_file_lines(const string &filename, const vector<string> &linesToWrite)
{
	ofstream of(filename.c_str());
	if(!of.is_open())
		return false;

	for (string line : linesToWrite)
		of << line << endl;

	return true;
}

vector<string> get_directory_content(const string &path)
{
	DIR* dir = opendir (path.c_str());
	struct dirent *ent;
	vector<string> directoryContent;

	if (dir)
	{
		while ((ent = readdir (dir)) != NULL)
		{
			string fileName = ent->d_name;
			if(fileName != "." && fileName != "..")
				directoryContent.push_back(fileName);
		}
	}

	closedir(dir);
	sort(directoryContent.begin(), directoryContent.end());

	return directoryContent;
}

bool append_to_wfile(const string& filename, const wstring& content)
{
	wofstream wof;
	wof.open(filename.c_str(), ios_base::app);
	if(!wof.is_open())
		return false;
	wof.imbue(locale(wof.getloc(), new codecvt_utf8<wchar_t>));
	wof << content;
	return true;
}

bool append_to_file(const string &filename, const string &content)
{
	ofstream of;
	of.open(filename.c_str(), ios_base::app);
	if(!of.is_open())
		return false;
	of << content;
	return true;
}

vector<string> explode(string const & s, char delim)
{
	vector<string> result;
	istringstream iss(s);

	for (string token; getline(iss, token, delim); )
	{
		result.push_back(move(token));
	}

	return result;
}

string absolute_path(string path)
{
	path = get_current_directory() + path;
	unique_ptr<char[]> absolutePath = unique_ptr<char[]>(new char[8129]);
	assert(CubicleSoft::UTF8::File::Realpath(absolutePath.get(), 8129, path.c_str()));
	return string(absolutePath.get());
}

pair<string, string> splitPathAndFilename(string path)
{
	vector<string> pathExploded = explode(path, '/');
	string filename = pathExploded.back();
	pathExploded.pop_back();
	string restOfPath = "";

	for (string dir : pathExploded)
		restOfPath += dir + "/";

	pair<string, string> pathAndFilename;
	pathAndFilename.first = restOfPath;
	pathAndFilename.second = filename;

	return pathAndFilename;
}

bool create_directory(string path)
{
	pair<string, string> pathAndFilename = splitPathAndFilename(path);

	path = absolute_path(pathAndFilename.first) + "/";
	path += pathAndFilename.second;
	return CubicleSoft::UTF8::Dir::Mkdir(path.c_str());
}

bool delete_directory(string path)
{
	path = absolute_path(path);
	return CubicleSoft::UTF8::Dir::Rmdir(path.c_str(), true);
}

bool delete_file(string path)
{
	path = absolute_path(path);
	return CubicleSoft::UTF8::File::Delete(path.c_str());
}

} //namespace
