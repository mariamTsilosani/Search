#ifndef INDEX_H
#define INDEX_H
#include <stdlib.h>

typedef struct node{
	char *word;
	struct node *next;
	struct fileNode *fileList;
}node;

typedef struct fileNode{
	char *fileName;
	int wordCount;
	struct fileNode *nextFile;
	struct fileNode *prevFile;
}fileNode;

typedef struct hashTable{
	int size;
	node **table;
}hashTable;

node* createList(char *word,char *filePath);

hashTable *createTable(int size);

node* addToList(char *word, node *head,char *filePath);

fileNode* sortFileName(fileNode* fileNode,char* filePath);

node* sortNode(node *head, char *word,char *filePath);

hashTable *hash(hashTable *tableOne,char *word, char *path);

void *freeList(node *head);

void *freeTable(hashTable *table);

void *fileParser(char *resultPath,char *path, hashTable *tableOne);

void *dirParser(char *resultPath, char *path, hashTable *tableOne);

void *input(char *path, hashTable *tableOne);


#endif
