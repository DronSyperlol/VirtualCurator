#include "Tools.h"

void delete_ptr(void* ptr)
{
	delete ptr;
	ptr = nullptr;
}
