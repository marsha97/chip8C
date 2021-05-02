struct _Memory
{
	unsigned char *base;
	void (*init)();
	int(*load)();
	void(*deinit)();
};

