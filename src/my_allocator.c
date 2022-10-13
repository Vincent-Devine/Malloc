#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "my_allocator.h"
#include "metadata.h"

static Metadata* firstMetadata = NULL; // First node

Metadata* mergeFreeBloc(Metadata* metadada)
{
	if(metadada->next != NULL && metadada->next->isFree) // Merge metabloc with the next
	{
		metadada->size += metadada->next->size + (int)sizeof(Metadata);
		metadada->next = metadada->next->next;
		if(metadada->next != NULL)
			metadada->next->prev = metadada;
	}
	if(metadada->prev != NULL && metadada->prev->isFree) // Merge metabloc with the prev
	{
		metadada->prev->size += metadada->size + sizeof(Metadata);
		metadada->prev->next = metadada->next;
		if(metadada->next != NULL)
			metadada->next->prev = metadada->prev;
		return metadada->prev;
	}
	return metadada;
}

Metadata* getFreeBloc(size_t size, Metadata** lastMetadata)
{
	for(Metadata* cur = firstMetadata; cur != NULL; cur = cur->next)
	{
		if(cur->isFree && cur->size >= (int)size)
		{
			return cur; // Return cur if their find a free bloc (with the good size)
		}
		*lastMetadata = cur;
	}
	return NULL; // Return NULL if their don't find a free bloc (with the good size)
}

Metadata* splitFreeBloc(int size, Metadata* freeMetadata)
{
	Metadata* newMetadata = freeMetadata->data + size;
	newMetadata->prev = freeMetadata;
	newMetadata->next = freeMetadata->next;
	freeMetadata->next = newMetadata;
	newMetadata->next->prev = newMetadata;
	newMetadata->size = freeMetadata->size - (size + sizeof(Metadata));
	freeMetadata->size = size;
	newMetadata->isFree = true;
	newMetadata->data = freeMetadata->data + size + sizeof(Metadata);
	return freeMetadata;
}

int matchingDataToEightBytes(int size)
{
	if(size % 8 != 0)
	{
		return (size + 8 - (size % 8));
	}
	else
	{
		return size;
	}
}

Metadata* searchMetadataWithDataPtr(void* ptr)
{
	for(Metadata* cur = firstMetadata; cur != NULL; cur = cur->next)
	{
		if(cur->data == ptr)
		{
			return cur;
		}
	}
	printf("[DEBUG - my_allocator.c - Metadata* searchMetadataWithDataPtr(void* ptr)] - No metadata found\n");
	return NULL;
}

void* my_malloc(size_t size)
{
	Metadata* freeMetadata = sbrk(0);
	Metadata* lastMetadata = sbrk(0);

	// Alignment data in Heaps
	size = matchingDataToEightBytes(size);
    // Verify space in Mapped Region
	freeMetadata = getFreeBloc(size, &lastMetadata);

	if(freeMetadata != NULL) // Has free bloc
	{
		//Split bloc
		if((size + sizeof(Metadata)) < (unsigned long)freeMetadata->size)
		{
			freeMetadata = splitFreeBloc(size, freeMetadata);
		}
		freeMetadata->isFree = false;
		return freeMetadata->data;
	}
    else // No free bloc found
	{
		if(firstMetadata != NULL)
		{
			Metadata* newMetadata = sbrk(sizeof(Metadata));
			newMetadata->next = NULL;
			newMetadata->isFree = false;
			newMetadata->size = size;
			newMetadata->data = sbrk(0);
			newMetadata->prev = lastMetadata;
			lastMetadata->next = newMetadata;
			sbrk(size);
			return newMetadata->data;
		}
		else // First metadata bloc
		{
			firstMetadata = sbrk(sizeof(Metadata));
			firstMetadata->prev = NULL;
			firstMetadata->data = sbrk(0);
			firstMetadata->isFree = false;
			firstMetadata->next = NULL;
			firstMetadata->size = size;
			sbrk(size);
			return firstMetadata->data;
		}
	}
}

void* my_realloc(void* ptr, size_t size)
{
	// Alignment data in Heaps
	size = matchingDataToEightBytes(size);

	Metadata* metadata = searchMetadataWithDataPtr(ptr);

	if(metadata->next != NULL && metadata->next->isFree)
	{
		if((metadata->next->size + sizeof(metadata) + metadata->size) >= size)
		{
			metadata->size = size;
			metadata->next = metadata->next->next;
			metadata->next->prev = metadata;
			return metadata->data;
		}
	}

	char* data = my_malloc(size);
	for(int i = 0; i < metadata->size; i++)
	{
		*(data+i) = *((char*)ptr+i);
	}
	my_free(ptr);
	return data;
}

void* my_calloc(size_t nb, size_t size)
{
    char* data =  my_malloc(nb * size);
	Metadata* metadata = searchMetadataWithDataPtr(data);

	for(int i = 0; i < metadata->size; i++)
	{
		*(data+i) = 0;
	}

	return data;
}

void my_free(void* ptr)
{
	Metadata* metadata = searchMetadataWithDataPtr(ptr);
	// Free the bloc
	metadata->isFree = true;
	// To merging the free bloc
	if((metadata->next != NULL && metadata->next->isFree) || (metadata->prev != NULL && metadata->prev->isFree))
	{
		metadata = mergeFreeBloc(metadata);
	}
	// To destroy the free bloc in the end of the node
	if(metadata->next == NULL)
	{
		if(metadata->prev != NULL)
		{
			metadata->prev->next = NULL;
		}
		else // Free firstmetadata
		{
			firstMetadata = NULL;
		}
		sbrk(-(metadata->size+sizeof(Metadata)));
	}

}

void showDebugInfo()
{
	for(Metadata* cur = firstMetadata; cur != NULL; cur = cur->next)
	{
		printf("curAdress = %p\n", cur);
		printf("curAdress->data = %p\n", cur->data);
		printf("curAdress->isFree = %d\n", cur->isFree);
		printf("curAdress->size = %d\n", cur->size);
		printf("curAdress->prev = %p\n", cur->prev);
		printf("curAdress->next = %p\n\n", cur->next);
	}
}