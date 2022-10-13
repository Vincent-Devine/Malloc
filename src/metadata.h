typedef struct Metadata
{
	int size;
	bool isFree;
	void* data;
	struct Metadata* next;
	struct Metadata* prev;
}Metadata;
