#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ElemType HuffmanTree

typedef struct TreeNode *HuffmanTree;
typedef char** HuffmanCode;
	
struct TreeNode {
	int Character;
	int Weight;
	int Flag;
	HuffmanTree Father;
	HuffmanTree Left, Right;
};

typedef struct HNode *Heap;
struct HNode {
    ElemType *Data; 
    int Size;         
    int Capacity;      
};

typedef Heap MinHeap;

int g_dictionary[256];
int g_times[256];
int g_number_of_character;

int Index(int c)
{
	for (int i=1; i<=g_number_of_character; i++)
	{
		if (g_dictionary[i] == c) return i;
	}
	g_dictionary[++g_number_of_character] = c;
	g_times[g_number_of_character] = 0;
	return g_number_of_character;
}

MinHeap CreateHeap( int MaxSize )
{ 
 
    MinHeap H = (MinHeap)malloc(sizeof(struct HNode));
    H->Data = (ElemType *)malloc((MaxSize+1)*sizeof(ElemType));
    H->Size = MaxSize;
    H->Capacity = MaxSize;
    H->Data[0] = NULL;
    
    for (int i=1; i<=MaxSize; i++)
	{
		H->Data[i] = (HuffmanTree)malloc(sizeof(TreeNode));
		
		H->Data[i]->Character = g_dictionary[i];
		H->Data[i]->Weight = g_times[i];
		
		H->Data[i]->Father = NULL;
		H->Data[i]->Left = NULL;
		H->Data[i]->Right = NULL;
		H->Data[i]->Flag = 0;
		
	}
 
    return H;
}
 
int IsFull( MinHeap H )
{
    return (H->Size == H->Capacity);
}
 
int Insert( MinHeap H, ElemType X )
{
    int i;
  
    if ( IsFull(H) ) { 
        printf("最小堆已满");
        return 0;
    }
    i = ++H->Size; 
    for ( ; i>1 && H->Data[i/2]->Weight>X->Weight; i/=2 )
        H->Data[i] = H->Data[i/2]; 
    H->Data[i] = X;
    return 1;
}
 
#define ERROR -1
 
int IsEmpty( MinHeap H )
{
    return (H->Size == 0);
}
 
ElemType DeleteMin( MinHeap H )
{ 
    int Parent, Child;
    ElemType MinItem, X;
 
    if ( IsEmpty(H) ) {
        printf("最小堆已为空");
        exit(ERROR);
    }
 
    MinItem = H->Data[1];
//    printf("%c %d\n", MinItem->Character, MinItem->Weight );
   
    X = H->Data[H->Size--]; 
    for( Parent=1; Parent*2<=H->Size; Parent=Child ) {
        Child = Parent * 2;
        if( (Child!=H->Size) && (H->Data[Child]->Weight>H->Data[Child+1]->Weight) )
        {
        	Child++;  
		}
        if( X->Weight >= H->Data[Child]->Weight )  
		{
			H->Data[Parent] = H->Data[Child];
		}
        else break;
    }
    H->Data[Parent] = X;
 
    return MinItem;
} 
 
void PercDown( MinHeap H, int p )
{
    int Parent, Child;
    ElemType X;
 
    X = H->Data[p]; 
    for( Parent=p; Parent*2<=H->Size; Parent=Child ) {
        Child = Parent * 2;
        if( (Child!=H->Size) && (H->Data[Child]->Weight>H->Data[Child+1]->Weight) )
        {
        	Child++; 
		}
        if( X->Weight >= H->Data[Child]->Weight )  
		{
			H->Data[Parent] = H->Data[Child];
		}
        else break;
    }
    H->Data[Parent] = X;
}
 
void BuildHeap( MinHeap H )
{
    int i;
 
    for( i = H->Size/2; i>0; i-- )
        PercDown( H, i );
}



HuffmanTree Huffman( MinHeap H )
{
	HuffmanTree T;
	BuildHeap(H);
	while (H->Size>1) { 
		T = (HuffmanTree)malloc( sizeof( struct TreeNode) );
		T->Father = NULL;
		T->Flag = 0;
		T->Character = 0;
		T->Left = DeleteMin(H);	
		T->Left->Father = T;
		T->Right = DeleteMin(H);	
		T->Right->Father = T;	
		T->Weight = T->Left->Weight+T->Right->Weight;
//		printf("%c%d\t%c%d\t%c%d\n",T->Character,T->Weight,T->Left->Character,T->Left->Weight,T->Right->Character,T->Right->Weight);
		Insert( H, T ); 
	}

	T = DeleteMin(H);
	return T;
}

HuffmanCode HuffmanCoding(HuffmanTree &HT, int n)
{	
	HuffmanCode HC = (HuffmanCode)malloc((n + 1) * sizeof(char *));
	char *cd = (char *)malloc((n + 1) * sizeof(char));
	
	HuffmanTree p = HT;
	int cdlen = 0;
	
	while (p)
	{
		if (p->Flag == 0)
		{
			p->Flag = 1;
			if (p->Left) 
			{
				p = p->Left;
				cd[cdlen++] = '0';
			}
			else if (p->Right == NULL)
			{
				int index = Index(p->Character);
				HC[index] = (char *)malloc((cdlen+1)*sizeof(char));
				cd[cdlen] = '\0';
				strcpy(HC[index], cd);
			}
		}
		else if (p->Flag == 1) 
		{
			p->Flag = 2;
			if (p->Left)
			{
				p = p->Right;
				cd[cdlen++] = '1';
			}
		}
		else
		{
			p->Flag = 0; 
			p = p->Father; 
			cdlen--;
		}
	}
	return HC;
}

void Init()
{
	g_dictionary[1] = EOF;
	g_times[1] = 0;
}

int main()
{
	Init();
	
	
	freopen("inp.txt", "r", stdin);
	int ch;
	while ( true )
	{
		ch=getchar();
		int index = Index(ch);
		g_times[index]++;
		if (ch == EOF) break;
		
	}
	fclose(stdin);

	
	MinHeap H = CreateHeap(g_number_of_character);
	HuffmanTree HT = Huffman(H);
	HuffmanCode HC = HuffmanCoding(HT, g_number_of_character);
	
		
	freopen("inp.txt", "r", stdin);
	freopen("outp.txt", "wb", stdout);
	char buffer = 0; 		//输出缓存 
	char *p;
	int time = 0;
	while ( ch=getchar() )
	{
		int index = Index(ch);
		
		p = HC[index];
		while (*p)
		{
			buffer = buffer << 1;
			time++;
			if (*p == '1') buffer |= 1;
			
			if (time == 8)
			{
				fwrite(&buffer, sizeof(char), 1, stdout);
				time = 0;
			}
			
			p++;
		}
		
		if (ch == EOF) 
		{
			if (time) 
			{
				while (time < 8)
				{
					time++;
					buffer = buffer << 1;
				}
				fwrite(&buffer, sizeof(char), 1, stdout);
			}
			break;
		}
		
	}
	fclose(stdin);
	fclose(stdout);
	
	
	freopen("outp.txt", "rb", stdin);
	freopen("rep.txt", "w", stdout);
	HuffmanTree node = HT;
	bool flag = true;
	while ( flag && fread(&buffer, sizeof(char), 1, stdin) )
	{	
		for (int time=0; flag && time<8; time++)
		{
			if ((buffer & 0x80) == 0) 
			{
				node= node->Left;
			}
			else 
			{
				node = node->Right;
			}
			
			if (node->Character > 0) 
			{
				printf("%c", node->Character);
				node = HT;
			}
			else if (node->Character == EOF)
			{
				flag = false;
			}
			
			buffer = buffer << 1;
		}
		
	}
	fclose(stdin);
	fclose(stdout);
}
