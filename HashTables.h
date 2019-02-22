/*-------------------------------------------------------------------
 *  Filename: HashTables.h
 *  Author: Smadu Razvan-Alexandru
 *-------------------------------------------------------------------
 */

#ifndef _HASH_TABLE_
#define _HASH_TABLE_

#include "lista.h"

// Hash function definition type
typedef unsigned int(*TFHash)(const void*, size_t, size_t);

// Display into file information function definition
typedef void(*fdisplay)(FILE*, const void*);

// Release memory function definition
typedef void(*release)(void*);

// Comparation function definition
typedef int(*compare)(const void*, const void*);

// Table hash structure
typedef struct th
{
	size_t m;		// capacity of the vector
	TFHash fh;		// hash function 
	TLG *bucket;		// vector of buckents
	fdisplay dKey;		// display the key function
	release rKey;		// release memory for key function
	compare cmpKey;		// compare 2 keys function
	int noItems;		// number of items
	float loadFactor;	// load factor
	void*(*fgetKey)(FILE*, size_t*);	// get key from file
}TH;

// Info structure
typedef struct
{
	fdisplay dispVal;	// display value function
	release freeVal;	// function for freeing the memory for value
	void* info;		// a pointer to information
	void* key;		// a pointer to key
	size_t sizeKey;		// size of the key
	fdisplay dKey;		// function to display the key
	release rKey;		// release memory for key (function)
	compare cmpKey;		// compare 2 keys function
}TInfo;


// Initializeaza tabela hash. Returneaza NULL pt esec sau o tabela hash pt succes
TH* initTH(size_t m, TFHash fh);

/*
  Insert element into hash table. These elements must be allocated before 
  calling this function
	returns -1 => alocation fail
	returns 0 => edited value
	returns 1 => new entry added
*/
int insTH(TH **th, void *adrEl, void *key, size_t sizeEl,
	int(*addElList)(ALG, void*, void*, compare));

// Print elements from hash table
void fprintTH(FILE* output, TH *th, void(*dispEl)(FILE*, const void*));

// Destroy an hash table and release memory 
void DestroyTH(TH **th, void(*freeEl)(void*));

// function for hashing the key
unsigned int hash_f(const void *data, size_t len, size_t range);

#endif
