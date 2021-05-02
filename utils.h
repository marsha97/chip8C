#include <stdlib.h>


void safeFree(void **p) {
	if (p != NULL && *p != NULL)
	{
		free(*p);
		*p = NULL;
	}
}