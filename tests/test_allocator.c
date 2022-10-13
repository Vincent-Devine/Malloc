#define DEFAULT_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <my_allocator.h>
 
//#define USE_MY_ALLOCATOR
 
#ifdef USE_MY_ALLOCATOR
#define ALLOC my_alloc
#define REALLOC my_realloc
#define CALLOC my_calloc
#define FREE my_free
#else
#define ALLOC malloc
#define REALLOC realloc
#define CALLOC calloc
#define FREE free
#endif

// Test step 4
void testMyMalloc()
{
	printf("-------- TestMyMalloc --------\n\n");
	int* a = my_malloc(sizeof(int));
	*a = 10;
	printf("(my_malloc) a = %d\n", *a);

	int* b = my_malloc(sizeof(int));
	*b = 15;
	printf("(my_malloc) b = %d\n", *b);

	int* c = my_malloc(sizeof(int));
	*c = 20;
	printf("(my_malloc) c = %d\n\n", *c);

	showDebugInfo();

	my_free(a);
	my_free(b);
	my_free(c);
}

// Test step 5
void testMyFree()
{
	printf("-------- TestMyFree --------\n\n");
	int* a = my_malloc(sizeof(int));
	*a = 10;
	printf("(my_malloc) a = %d\n", *a);
	int* b = my_malloc(sizeof(int));
	*b = 15;
	printf("(my_malloc) b = %d\n", *b);
	int* c = my_malloc(sizeof(int));
	*c = 20;
	printf("(my_malloc) c = %d\n", *c);

	my_free(b);
	printf("my_free(b)\n");

	showDebugInfo();

	my_free(a);
	my_free(c);
}

// Test step 5
void testReuseFreeSpace()
{
	printf("-------- testReuseFreeSpace --------\n\n");
	int* a = my_malloc(sizeof(int));
	*a = 10;
	printf("(my_malloc) a = %d\n", *a);
	int* b = my_malloc(sizeof(int));
	*b = 15;
	printf("(my_malloc) b = %d\n", *b);
	int* c = my_malloc(sizeof(int));
	*c = 20;
	printf("(my_malloc) c = %d\n", *c);

	my_free(b);
	printf("my_free(b)\n");

	showDebugInfo();

	int* d = my_malloc(sizeof(int));
	*d = 25;
	printf("(my_malloc) d = %d\n", *d);

	showDebugInfo();

	my_free(a);
	my_free(c);
	my_free(d);
}

// Test step 6
void testSplitBloc()
{
	printf("-------- testFreeBlocAndSplit --------\n\n");
	char* c = my_malloc(128 * sizeof(char));
	*c = 'c';
	printf("(my_malloc) c = %c\n", *c);

	int* a = my_malloc(sizeof(int));
	*a = 15;
	printf("(my_malloc) a = %d\n\n", *a);

	showDebugInfo();

	my_free(c);
	printf("my_free(c)\n\n");

	showDebugInfo();

	int* b = my_malloc(sizeof(int));
	*b = 20;
	printf("(my_malloc) b = %d\n\n", *b);

	showDebugInfo();

	int* d = my_malloc(sizeof(int));
	*d = 25;
	printf("(my_malloc) d = %d\n\n", *d);

	showDebugInfo();

	my_free(a);
	my_free(b);
	my_free(d);
}

// Test step 7
void testMergeBloc()
{
	printf("-------- testMergeBloc --------\n\n");
	int* a = my_malloc(sizeof(int));
	*a = 10;
	printf("(my_malloc) a = %d\n", *a);
	int* b = my_malloc(sizeof(int));
	*b = 15;
	printf("(my_malloc) b = %d\n", *b);
	int* c = my_malloc(sizeof(int));
	*c = 20;
	printf("(my_malloc) c = %d\n", *c);

	int* d = my_malloc(sizeof(int));
	*d = 25;
	printf("(my_malloc) d = %d\n", *d);

	int* e = my_malloc(sizeof(int));
	*e = 30;
	printf("(my_malloc) e = %d\n\n", *e);

	showDebugInfo();

	my_free(d);
	my_free(c);

	printf("my_free(d) + my_free(c)\n\n");

	showDebugInfo();

	my_free(a);
	my_free(b);

	printf("my_free(a) + my_free(b)\n\n");

	showDebugInfo();

	my_free(e);
}

// Test step 8
void testMyCalloc()
{
	printf("-------- testMyCalloc --------\n\n");
	char* c = my_calloc(15, sizeof(char));
	printf("(my_calloc) c = %c\n", *c);

	int* a = my_calloc(1, sizeof(int));
	printf("(my_calloc) a = %d\n", *a);

	printf("\n");
	showDebugInfo();

	my_free(a);
	my_free(c);
}

// Test step 9
void testMyRealloc()
{
	printf("-------- testMyRealloc --------\n\n");
	int* a = my_malloc(sizeof(int));
	*a = 10;
	printf("(my_malloc) a = %d\n", *a);
	int* b = my_malloc(sizeof(int));
	*b = 15;
	printf("(my_malloc) b = %d\n", *b);
	int* c = my_malloc(sizeof(int));
	*c = 20;
	printf("(my_malloc) c = %d\n\n", *c);

	showDebugInfo();

	printf("my_realloc(b, 2*sizeof(int))\n\n");
	b = my_realloc(b, 4*sizeof(int));

	showDebugInfo();

	printf("my_realloc(a, 2*sizeof(int))\n\n");
	a = my_realloc(a, 4*sizeof(int));

	showDebugInfo();

	printf("a = %d, b = %d, c = %d\n\n", *a, *b, *c);

	my_free(a);
	showDebugInfo();
	my_free(b);
	my_free(c);
}

// Test step 10
void testAlignmentData()
{
	printf("-------- testAlignmentData --------\n\n");
	int* a = my_malloc(sizeof(int)); // 4 bytes
	*a = 10;
	printf("(my_malloc) a = %d\n", *a);
	int* b = my_malloc(2*sizeof(int)); // 8 bytes
	*b = 15;
	printf("(my_malloc) b = %d\n\n", *b);

	showDebugInfo();

	my_free(a);
	my_free(b);
}

// Test step 11
void testReduceBloc()
{
	printf("-------- testReduceBloc --------\n\n");
	int* a = my_malloc(sizeof(int));
	*a = 10;
	printf("(my_malloc) a = %d\n", *a);
	int* b = my_malloc(sizeof(int));
	*b = 15;
	printf("(my_malloc) b = %d\n\n", *b);

	showDebugInfo();

	printf("my_free(b)\n\n");
	my_free(b);

	showDebugInfo();

	my_free(a);
}

int main()
{
	void* initAdress = sbrk(0);
	printf("Init adress : %p\n\n", initAdress);

	// Test step 4
	testMyMalloc();

	// Test du free (step 5)
	//testMyFree();

	// Test step 5
	//testReuseFreeSpace();
	
	// Test step 6
	//testSplitBloc();

	// Test step 7
	//testMergeBloc();

	// Test step 8
	//testMyCalloc();

	// Test step 9
	//testMyRealloc();

	// Test step 10
	//testAlignmentData();

	// Test step 11
	//testReduceBloc();
 
   return 0;
}
