/*-------------------------------------------------------------------
 *  Filename: HashTables.c
 *  Author: Smadu Razvan-Alexandru
 *-------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include "HashTables.h"

TH* initTH(size_t m, TFHash fh)
{
	// alloc memory for hash table
	TH *th = (TH*)malloc(sizeof(TH));

	if (!th)
		return NULL;

	// alloc memory for buckets
	th->bucket = (TLG*)calloc(m, sizeof(TLG));
	if (!th->bucket)
	{
		free(th);
		return NULL;
	}

	// initialize records form structure
	th->m = m;
	th->fh = fh;
	th->cmpKey = NULL;
	th->dKey = NULL;
	th->rKey = NULL;
	th->noItems = 0;
	th->loadFactor = 0;

	return th;
}

int insTH(TH **th, void *adrEl, void *key, size_t sizeKey, 
	int(*addElList)(ALG, void*, void*, compare))
{
	size_t cod = (*th)->fh(key, sizeKey, (*th)->m);
	TLG *l = &(*th)->bucket[cod];

	int result = addElList(l, adrEl, key, (*th)->cmpKey);
	if (result == 1)
		return 1;
	else if (result == 0)
		return 0;
	return -1;
}

void fprintTH(FILE* output, TH *th, void(*dispEl)(FILE*, const void*))
{
	size_t cod;
	for (cod = 0; cod < th->m; cod++)
	{
		fprintf(output, "[%d] :", cod);
		TLG l = th->bucket[cod];
		for (; l != NULL; l = l->urm)
			dispEl(output, l->info);
		fprintf(output, "\n");
	}
}

void DestroyTH(TH **th, void(*freeEl)(void*))
{
	if (!*th)
		return;
	TLG p, aux;
	size_t cod;
	for (cod = 0; cod < (*th)->m; cod++)
	{
		// free information stored in buckets
		p = (*th)->bucket[cod];
		while(p != NULL)
		{
			aux = p;
			p = p->urm;
			freeEl(aux->info);
			free(aux);
		}
	}

	// free the vector of buckets
	free((*th)->bucket);

	// free hash table
	free(*th);
	*th = NULL;
}
