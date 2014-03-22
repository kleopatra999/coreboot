#include <stddef.h>
#include <cbmem.h>
#include "cpu.h"
void *cbmem_top(void)
{
	return (void *) ( RAM_BASE_KB + RAM_SIZE_KB - FB_SIZE_KB);
}

