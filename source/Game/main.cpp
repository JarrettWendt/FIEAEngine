// Game.Windows.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <concepts>
#include <vector>

#include "Engine.h"
#include "Library.Windows.h"
#include "Util.h"

using namespace Library;

int main()
{
	std::cout << "Hello CMake." << std::endl;
	std::cout << std::boolalpha << std::integral<int> << std::endl;
	std::cout << Library::EngineFunc() << std::endl;
	std::cout << Library::WindowsOnlyFunc() << std::endl;

	std::cout << std::boolalpha << Util::IsOneOf<int, bool, int, float, std::string>() << std::endl;

	std::vector<int> v{ 0, 1 ,2 ,3, 4, };
	std::cout << std::boolalpha << Util::Contains(v, 7) << std::endl;
	std::cout << std::boolalpha << Util::Contains(v, 4) << std::endl;

#ifndef _WINDOWS
	while (getchar());
#endif
	
	return 0;
}
