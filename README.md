# Malloc project

## Presentation
Malloc project for ISART.\
This project have to goal to recoding basic function used in C langage. Malloc, Calloc, Realloc and Free.

### Malloc
Malloc() function allocates size bites and returns a pointer to the allocated memory.\
The memory is not initialized.

### Calloc
Malloc() function allocates allocated memory for an array of nmemb elemets of size bytes and returns a pointer to the allocated memory.\
The memory is set to zero.

### Reacloc
Realloc() function changes the size of the memory block pointed to by ptr to size bytes.\
The contents will be unchanged in the range from the start of the region up to the minimum of the old and new sizes. If the new size is larger  than  the old size, the added memory will not be initialized. 

### Free
Free() function frees the memory space pointed to by ptr.\
</br>

## Infomation about the project
project start : 10/06/2021\
project end : 10/14/2021\
vesion : 1.0\
date last version : 10/13/2021\
development team :
- Vincent DEVINE
</br>

## Command
### Create an executable
```sh
$make
```
### Execute the executable
```sh
$./test_allocator
```
### Clean the project
```sh
$make clean
```
</br>

## Project structure
```sh
├─ Makefile
│
├─ README.md
│
├─ include
│   └─ my_allocator.h
│
├─ src
│   ├── metadata.h
│   └─ my_allocator.c
│
└─ tests
    ├── test_allocator.c
    └─ test_brk.c
```

## Contact
mail : v.devine@student.isartdigital.com
