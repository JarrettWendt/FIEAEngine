// To change this to a Windows Desktop Application:
// Go to Project Properties > Linker > System > SubSystem
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c
// Change main to WinMain

#include "pch.h"

#include "Datum.h"
#include "SList.h"
#include "HashMap.h"
#include "Array.h"
#include <forward_list>

using namespace Library;

int main()
{
	std::cout << "string " << sizeof(std::string) << std::endl;
	std::cout << "string::iterator " << sizeof(std::string::iterator) << std::endl;
	std::cout << std::endl;
	
	std::cout << "forward_list " << sizeof(std::forward_list<int>) << std::endl;
	std::cout << "SList " << sizeof(SList<int>) << std::endl;
	std::cout << "forward_list::iterator " << sizeof(std::forward_list<int>::iterator) << std::endl;
	std::cout << "SList::iterator " << sizeof(SList<int>::iterator) << std::endl;
	std::cout << std::endl;
	
	std::cout << "vector " << sizeof(std::vector<int>) << std::endl;
	std::cout << "Array " << sizeof(Array<int>) << std::endl;
	std::cout << "vector::iterator " << sizeof(std::vector<int>::iterator) << std::endl;
	std::cout << "Array::iterator " << sizeof(Array<int>::iterator) << std::endl;
	std::cout << std::endl;

	std::cout << "unordered_map " << sizeof(std::unordered_map<std::string, int>) << std::endl;
	std::cout << "HashMap " << sizeof(HashMap<std::string, int>) << std::endl;
	std::cout << "unordered_map::iterator " << sizeof(std::unordered_map<std::string, int>::iterator) << std::endl;
	std::cout << "HashMap::iterator " << sizeof(HashMap<std::string, int>::iterator) << std::endl;
	std::cout << std::endl;

	std::cout << "VariantArray " << sizeof(VariantArray<bool, int, float, std::string>) << std::endl;
	std::cout << "VariantArray::iterator " << sizeof(VariantArray<bool, int, float, std::string>::iterator) << std::endl;
	std::cout << "Datum " << sizeof(Datum) << std::endl;
	std::cout << "Datum::iterator " << sizeof(Datum::iterator) << std::endl;
	std::cout << std::endl;

	return 0;
}
