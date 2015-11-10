#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	unsigned int weight;
	unsigned int parent, lchild, rchild;
}HTNode, *HuffmanTree;

typedef char** HuffmanCode;

void Select(HuffmanTree &HT, int n, int *s1, int *s2)
{
}

void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int *w, int n)
{
	if (n <= 1) return;
	int m = 2 * n - 1;
	HT = (HuffmanTree)malloc((m+1)*sizeof(HTNode));
	
	unsigned int i;
	HuffmanTree p;
	for (p=HT, i=1; i<=n; i++, p++, w++)
	{
		p->weight = *w;
		p->lchild = 0;
		p->rchild = 0;
		p->parent = 0;
	}
	for (; i<=m; i++, p++)
	{
		p->weight = 0;
		p->lchild = 0;
		p->rchild = 0;
		p->parent = 0;
	}
	for (i=n+1; i<=m; i++)
	{
		int s1, s2;
		Select(HT, i-1, &s1, &s2);
		HT[s1].parent = i;
		HT[s2].parent = i;
		HT[i].lchild = s1;
		HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	
	HC = (HuffmanCode)malloc((n + 1) * sizeof(char *));
	char *cd = (char *)malloc(n * sizeof(char));
	cd[n-1] = '\0';
	for (i=1; i<=n; i++)
	{
		int start = n - 1;
		for (int c=i, f=HT[i].parent; f!=0; c=f, f=HT[f].parent)
		{
			if (HT[f].lchild == c)
			{
				cd[--start] = '0';
			}
			else 
			{
				cd[--start] = '1';
			}
		}
		HC[i] = (char*) malloc((n-start)*sizeof(char));
		strcpy(HC[i], &cd[start]);
	}
	free(cd);
}
