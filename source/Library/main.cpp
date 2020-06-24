#include "Engine.h"

int main(const int argc, const char* argv[])
{
	Library::Engine::Main({ argv, size_t(argc) });
	return 0;
}
