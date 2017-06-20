#ifndef WFILEIO_H
#define WFILEIO_H

#include <string>

using namespace std;

extern "C++"
{
namespace RDI
{
std::wstring readWFile(string filename);
bool writeWFile(string filename, wstring fileContent);
std::string readFile(string filename);
bool writeFile(string filename, string fileContent);
string getCurrentDirectory(); // returns the location of the binary ex "/home/rdi/bin"
}
} // namespace

#endif // WFILEIO_H
