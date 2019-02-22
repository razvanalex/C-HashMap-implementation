/*-------------------------------------------------------------------
 *  Filename: main.c
 *  Author: Smadu Razvan-Alexandru
 *-------------------------------------------------------------------
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "HashTables.h"
#include "materie.h"
#include "student.h"

#define LFTHRESHOLD 0.75 

// print TInfo information into file
void printEl(FILE *output, const void *info)
{
	TInfo *i = ((TInfo*)info);

	fprintf(output, " (");
	i->dKey(output, i->key);
	fprintf(output, " -> ");
	i->dispVal(output, i->info);
	fprintf(output, ")");
}

// print student info to output file
void printStud(FILE *output, const void *el)
{
	TStudent* stud = (TStudent*)el;

	if (!stud)
		return;
	
	fprintf(output, "[");
	fprintf(output, "Nume: %s, ", stud->nume);
	fprintf(output, "Grupa: %s, ", stud->grupa);
	fprintf(output, "Medie: %.2f, ", stud->medie);
	fprintf(output, "Varsta: %d", stud->varsta);
	fprintf(output, "]");
}

// print materie info to output file
void printMat(FILE *output, const void *el)
{
	TMaterie* mat = (TMaterie*)el;

	if (!mat)
		return;

	fprintf(output, "[");
	fprintf(output, "Materie: %s, ", mat->materie);
	fprintf(output, "Ore_curs: %d, ", mat->ore_curs);
	fprintf(output, "Ore_lab: %d, ", mat->ore_lab);
	fprintf(output, "Credit: %d, ", mat->credit);
	fprintf(output, "Teme: %d", mat->teme);
	fprintf(output, "]");
}

// Get information of TStudent from input file
TStudent* ReadStudFromFile(FILE* file)
{
	TStudent *stud = (TStudent*)malloc(sizeof(TStudent));
	if (!stud)
		return NULL;

	// create a small buffer to store the name
	char nume[100];

	fscanf(file, " %s", nume);
	stud->nume = strdup(nume);
	fscanf(file, "%f", &stud->medie);
	fscanf(file, " %s", stud->grupa);
	fscanf(file, " %d", &stud->varsta);

	return stud;
}

// Get information form TMaterie from input file
TMaterie* ReadMatFromFile(FILE* file)
{
	TMaterie *mat = (TMaterie*)malloc(sizeof(TMaterie));
	if (!mat)
		return NULL;
	
	// create a small buffer to store the name
	char nume[100];

	fscanf(file, " %s", nume);
	mat->materie = strdup(nume);
	fscanf(file, "%d", &mat->ore_curs);
	fscanf(file, "%d", &mat->ore_lab);
	fscanf(file, "%d", &mat->credit);
	fscanf(file, "%d", &mat->teme);

	return mat;
}

// generate a cell that stores key, info, and pointers to functions
TInfo* generateInfoCell(void* el, void* key,
	fdisplay dispVal, release freeVal,
	fdisplay dispKey, release freeKey, 
	compare cmpKey, size_t sizeKey)
{
	TInfo *info = (TInfo*)malloc(sizeof(TInfo));
	if (!info)
		return NULL;

	info->info = el;
	info->key = key;

	info->sizeKey = sizeKey;
	info->dispVal = dispVal;
	info->freeVal = freeVal;

	info->cmpKey = cmpKey;
	info->dKey = dispKey;
	info->rKey = freeKey;

	return info;
}

// Free information from TInfo
void FreeInfo(void* info)
{
	TInfo *i = (TInfo*)info;
	i->freeVal(i->info);
	i->rKey(i->key);

	free(info);
}

// Free information form TMaterie
void FreeMat(void *mateier)
{
	TMaterie* mat = (TMaterie*)mateier;
	if (mat)
	{
		if (((TMaterie*)mat)->materie)
			free(((TMaterie*)mat)->materie);

		free(((TMaterie*)mat));
	}
}

// Free information from TStudent
void FreeStud(void *student)
{
	TStudent* stud = (TStudent*)student;

	if (stud)
	{
		if (((TStudent*)stud)->nume)
			free(((TStudent*)stud)->nume);

		free(((TStudent*)stud));
	}
}

// Print an integer to a file
void fprintInt(FILE* file, const void *el)
{
	fprintf(file, "%d", *(int*)el);
}

// Print a string to a file
void fprintString(FILE* file, const void *el)
{
	fprintf(file, "%s", (char*)el);
}

// Compare two integers
int cmpInt(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

// Alloc a cell for TLG list. 
TLG AlocCelula(void *ae)
{
	TLG aux;
	aux = (TLG)malloc(sizeof(TCelulaG));
	if (!aux)
		return NULL;

	aux->urm = NULL;
	aux->info = ae;

	return aux;
}


// Add element to TLG list.
int addElList(ALG th, void* el, void *key, compare cmp)
/*
	returns -1 => alocation fail
	returns 0 => edited value
	returns 1 => new entry added
*/
{
	TLG p = *th, ant = NULL, aux;

	// Find an existing key
	while (p)
	{
		TInfo *i = p->info;
		if (!cmp(i->key, key))
			break;
			
		ant = p;
		p = p->urm;
	}

	// alloc an aux cell
	aux = AlocCelula(el);
	if (!aux)
		return -1;
	
	// Remake the links
	if (ant)
		ant->urm = aux;
	else
		*th = aux;
	
	// Rewrite information
	if (p)
	{
		aux->urm = p->urm;
		FreeInfo(p->info);
		free(p);
		return 0;
	}

	return 1;
}

// Get Key from file as int
void* fgKeyInt(FILE *input, size_t *len)
{
	void *key;
	int key_int;

	fscanf(input, " %d", &key_int);	
	
	// alloc memory for key and copy it
	key = (void*)malloc(sizeof(int));
	if (key)
		memcpy(key, &key_int, sizeof(int));
	else 
		return NULL;

	// compute sizeo of key
	*len = sizeof(key_int);

	return key;
}

// Get Key from file as string
void* fgKeyString(FILE *input, size_t *len)
{
	void *key;
	char key_str[50];
	
	fscanf(input, " %s", key_str);
	
	// alloc memory for key and copy it
	key = strdup(key_str);
	if (!key)
		return NULL;
	
	// compute sizeo of key
	*len = strlen(key_str) + 1;
	
	return key;
}

// Get Data (TStudent info or TMat info) from file
void finsertTH(FILE *input, TH *th)
{
	size_t len;
	TInfo *i;
	char type_info[5];
	void *key = NULL;

	// get the key from file
	key = th->fgetKey(input, &len);

	fscanf(input, " %s", type_info);

	// Get TStudent data
	if (!strcmp(type_info, "stud"))
	{
		TStudent *s = ReadStudFromFile(input);
		i = generateInfoCell(s, key,
			printStud, FreeStud,
			th->dKey, th->rKey,
			th->cmpKey, len);
	}
	// Get TMaterie data
	else if (!strcmp(type_info, "mat")) 
	{
		TMaterie *m = ReadMatFromFile(input);
		i = generateInfoCell(m, key,
			printMat, FreeMat,
			th->dKey, th->rKey,
			th->cmpKey, len);
	}

	// Add the cell generated to TH and increase 
	// the number of elements stored + compute load factor
	if (i)
		if (insTH(&th, (void*)i, (void*)i->key, i->sizeKey,
			addElList))
		{
			th->noItems++;
			th->loadFactor = (float)th->noItems /
				(float)th->m;
		}
}

// Resize the table hash
int resizeTH(TH *th)
{
	// Double the size of vector of buckets 
	// and alloc memory for it
	size_t newSize = th->m * 2;
	TLG *newBucket = (TLG*)calloc(newSize, sizeof(TLG));
	if (!newBucket)
		return 0;

	// Get every element from the old vector and 
	// compute where to put it + remake the bounds
	size_t cod, newCode;
	for (cod = 0; cod < th->m; cod++)
	{
		TLG *l = &th->bucket[cod];
		while(*l != NULL)
		{
			// This is an alias to info 
			TInfo *info = (TInfo*)(*l)->info;

			// Find where to put the cell
			newCode = th->fh(info->key, info->sizeKey, newSize);
			TLG newL = newBucket[newCode], ultim = NULL, aux;
			while (newL != NULL)
			{
				ultim = newL;
				newL = newL->urm;
			}

			// Add cell (TInfo) to list
			if (ultim == NULL)
				newBucket[newCode] = *l;
			else ultim->urm = *l;

			aux = (*l)->urm;
			(*l)->urm = NULL;

			*l = aux;
		}


	}

	// update data from TH (size, load factor, vector of buckets)
	th->m = newSize;
	th->loadFactor = (float)th->noItems /
		(float)th->m;
	free(th->bucket);
	th->bucket = newBucket;

	return 1;

}

// Find info using the key
TInfo* findInfo(TH *th, void *key, size_t sizeKey)
{
	size_t cod = th->fh(key, sizeKey, th->m);
	TLG l = th->bucket[cod];
	TInfo *info = NULL;

	for (; l != NULL; l = l->urm)
	{
		info = (TInfo*)l->info;
		// if the key is found, return it's address
		if (info && !info->cmpKey(info->key, key))
			return info;
	}

	// here the key is not found
	return NULL;
}


// delete information (TLG cell) from TH 
int deleteInfo(TH *th, void *key, size_t sizeKey)
{
	size_t cod = th->fh(key, sizeKey, th->m);
	TLG l = th->bucket[cod], ant = NULL;
	TInfo *info = NULL;

	// find element by key
	for (; l != NULL; ant = l, l = l->urm)
	{
		info = (TInfo*)l->info;
		if (info)
			if (!info->cmpKey(info->key, key))
				break;

	}

	// if element is found
	if (l)
	{
		// remake links
		if (ant)
			ant->urm = l->urm;
		else th->bucket[cod] = l->urm;

		// update date from TH
		th->noItems--;
		th->loadFactor = (float)th->noItems /
			(float)th->m;

		// release memory
		info->freeVal(info->info);
		info->rKey(info->key);
		free(info);
		free(l);

		// success
		return 1;
	}

	// fail
	return 0;
	
}

// Get commands from file and interpret them
void ReadDataFromFile(FILE *input, FILE *output, TH **th)
{
	char operation[10], type_key;
	int no_buckets, nrOp, crt_op;
	void *key;
	size_t sizekey;
	TInfo *info;

	// get init command and init TH. 
	fscanf(input, "%s", operation);
	if (!strcmp(operation, "inithash"))
	{
		fscanf(input, " %c", &type_key);
		fscanf(input, " %d", &no_buckets);
		*th = initTH(no_buckets, hash_f);
		// int keys
		if (type_key == 'd')
		{		
			(*th)->cmpKey = cmpInt;
			(*th)->dKey = fprintInt;
			(*th)->rKey = free;
			(*th)->fgetKey = fgKeyInt;
		}
		// string keys
		else if (type_key == 's')
		{
			(*th)->cmpKey = (compare)strcmp;
			(*th)->dKey = fprintString;
			(*th)->rKey = free;
			(*th)->fgetKey = fgKeyString;
		}
	}
	else
	{
		printf("This file does not meet the requirements!\n");
		return;
	}
	fscanf(input, "%d", &nrOp);

	// read nrOp operations from input file
	for (crt_op = 0; crt_op < nrOp; crt_op++)
	{
		// read command
		fscanf(input, "%s", operation);

		// interpret command
		if (!strcmp(operation, "insert"))
		{
			// insert
			finsertTH(input, *th);

			// check for resize
			if ((*th)->loadFactor >= LFTHRESHOLD)
				resizeTH(*th);
		}
		else if (!strcmp(operation, "print"))
		{
			// print
			fprintTH(output, *th, printEl);
		}
		else if (!strcmp(operation, "find"))
		{
			// get key from file
			key = (*th)->fgetKey(input, &sizekey);
			
			// find key
			if ((info = findInfo(*th, key, sizekey)))
			{
				info->dispVal(output, info->info);
				fprintf(output, "\n");
			}
			else fprintf(output, "Nu exista\n");

			// free now the key
			free(key);
		}
		else if (!strcmp(operation, "delete"))
		{
			// get key from file
			key = (*th)->fgetKey(input, &sizekey);
			
			// delete information
			deleteInfo(*th, key, sizekey);

			// free key to avoid memory leaks
			free(key);
		}
	}
	// to be sure that there will not
	// be problems with key
	key = NULL;
}

int main()
{
	TH *th = NULL;
	
	FILE *input = fopen("input.in", "rt");
	FILE *output = fopen("output.out", "wt");

	if (input && output)
	{
		// interpret data from file and create TH
		// plus other operations
		ReadDataFromFile(input, output, &th);

		// finally destroy TH when not using it
		DestroyTH(&th, FreeInfo);	

		// close files
		fclose(input);
		fclose(output);
	}
	else 
	{
		// There is a problem with the input file
		printf("Error while opening the file...\n");
		if (output)
			fclose(output);
		if (input)
			fclose(input);
	}

	return 0;
}
