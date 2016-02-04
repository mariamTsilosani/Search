#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>


/*
 * Mariam Tsilosani 
 */

/*The three struct definitions on the bottom:
 *  the first one "node" is the struct of nodes that hold the words read from the file,
 *  the second "fileNode" is a struct of nodes that hold the file names the word was found in
 *  and the third one "hashTable" is a hashtable that holds nodes
 *  */
typedef struct node{
	char* word;

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

/*create table creates a hash table of size 36 mallocking size for the new table
 * and every node (head of the list) that will go in the table
 */ 
hashTable *createTable(int size){
	hashTable *newTable;

	if(size < 1){
		printf("invalide table size");
		return NULL;
	}

	if((newTable = malloc(sizeof(hashTable))) == NULL){
		printf("couldn't allocate memory");
		return NULL;
	}

	if((newTable->table = malloc(sizeof(node)*size)) == NULL){
		printf("couldn't allocate memory");
		return NULL;
	}
	int i = 0;
	for(i=0;i<size;i++){
		newTable->table[i] = NULL;
	}	

	newTable->size = size;

	return newTable;
}

/*createList creates a new list of nodes that will be stored in the hash table later on*/
node* createList(char* word, char* filePath){
	node *head;
	head = (node *)malloc(sizeof(node));
	if(head == NULL){
		return NULL;	
	}
	head->word = malloc((strlen(word)+ 1) *sizeof(char));
	strcpy(head->word,word);
	head->next = NULL;
	head->fileList = malloc(sizeof(fileNode));
	head->fileList->fileName = malloc((strlen(filePath) + 1) * sizeof(char));
	strcpy(head->fileList->fileName,filePath);

	head->fileList->wordCount = 1;
	head->fileList->nextFile = NULL;
	return head;
}

/*sort node receives all the info for a new node to be inserted
 * finds a right spot to insert it in (descending order) and inserts
 * it into a linked list that is also provided to the method*/
node* sortNode(node *head, char *word, char *filePath){
	if(head == NULL || word == NULL || filePath == NULL){
		return head;
	}


	node *prev;
	node *curr;
	prev = head;
	curr = head;
	node *newNode;
	newNode = (node *)malloc(sizeof(node));
	if(newNode == NULL){
		return NULL;
	}

	if(strcmp(head->word,word) > 0){

		newNode->word = malloc((strlen(word) + 1) * sizeof(char));
		strcpy(newNode->word, word);
		fileNode *newFileNode;
		newFileNode = (fileNode *)malloc(sizeof(fileNode));
		newFileNode->fileName = malloc((strlen(filePath) + 1) *sizeof(char));
		strcpy(newFileNode->fileName,filePath);
		newFileNode->wordCount = 1;
		newFileNode->nextFile = NULL;
		newFileNode->prevFile = NULL;
		newNode->fileList = newFileNode;
		newNode->next = head;
		head = newNode;
		return head;
	}
	curr = curr->next;
	while(curr != NULL){
		if(strcmp(curr->word,word) > 0){

			newNode->word = malloc((strlen(word) + 1) * sizeof(char));
			strcpy(newNode->word, word);
			fileNode *newFileNode;
			newFileNode = (fileNode *)malloc(sizeof(fileNode));
			newFileNode->fileName = malloc((strlen(filePath) + 1) *sizeof(char));
			strcpy(newFileNode->fileName,filePath);
			newFileNode->wordCount = 1;
			newFileNode->nextFile = NULL;
			newFileNode->prevFile = NULL;
			newNode->fileList = newFileNode;
			newNode->next = curr;
			prev->next = newNode;
			//head = newNode;
			return head;
		}
		prev = curr;
		curr = curr->next;

	}

	newNode->word = malloc((strlen(word) + 1) * sizeof(char));
	strcpy(newNode->word, word);
	fileNode *newFileNode;
	newFileNode = (fileNode *)malloc(sizeof(fileNode));
	newFileNode->fileName = malloc((strlen(filePath) + 1) *sizeof(char));
	strcpy(newFileNode->fileName,filePath);
	newFileNode->wordCount = 1;
	newFileNode->nextFile = NULL;
	newFileNode->prevFile = NULL;
	newNode->fileList = newFileNode;
	newNode->next = NULL;
	prev->next = newNode;
	return head;


}

/* sortFileName receives a file path and a linked list of fileNode structs
 * looks if the file name exists, if it exists it increments the wordcount 
 * and moves the existing node to a right location, if the filename doesn't 
 * exist then it makes a new fileNode and attaches it to the end of the list
 */	


fileNode *sortFileName(char* filePath,fileNode* node){
	
	if(filePath == NULL || strlen(filePath) == 0 || node == 0){
		return node;
	} 
	if(strcmp(node->fileName,filePath) == 0){
		node->wordCount++;
		return node;
	}

	fileNode *curr;
	curr = node;
	fileNode *prev;
	prev = node;
	while(curr != NULL){
		if(strcmp(curr->fileName,filePath) == 0){
			curr->wordCount++;
			break;
		}
		prev = curr;
		curr = curr->nextFile;
	}

	if(curr == NULL){

		prev->nextFile = malloc(sizeof(fileNode));
		int pathSize = strlen(filePath);
		prev->nextFile->fileName =(char*)malloc(pathSize + 1);
		strcpy(prev->nextFile->fileName,filePath);
		prev->nextFile->wordCount = 1;
		prev->nextFile->nextFile = NULL;
		prev->nextFile->prevFile = prev;

		return node;
	}
	while(prev != NULL){
		if(curr->wordCount > prev->wordCount){
			prev = prev->prevFile;
			continue;
		}
		break;
	}

	curr->prevFile->nextFile = curr->nextFile;
	if(curr->nextFile != NULL){
		curr->nextFile->prevFile = curr->prevFile;
	}
	if(prev == NULL){
		curr->nextFile = node;
		curr->prevFile = NULL;
		node->prevFile = curr;
		node = curr;
	}
	else{
		curr->nextFile = prev->nextFile;
		curr->prevFile = prev;
		if(curr->nextFile != NULL){
			curr->nextFile->prevFile = curr;
		}
		prev->nextFile = curr;	
	}
	return node;
}
/*addToList gets all the info for a new node and adds it to the existing list
 * at a corresponding spot (calling sortNode if necessary)
 */
node* addToList(char* word, node *head, char* filePath){

	if(head == NULL || word == NULL){
		return NULL;
	}
	node *temp = head;

	if(strcmp(temp->word,word) == 0){		
		temp->fileList = sortFileName(filePath,temp->fileList);
		return head;
	}
	node *prev;
	prev = temp;
	temp = temp->next;
	while(temp != NULL){
		if(strcmp(temp->word,word) == 0){

			temp->fileList = sortFileName(filePath,temp->fileList);
			return head;
		}
		prev = temp;
		temp = temp->next; 
	}
	head = sortNode(head,word,filePath);
	return head;

}

/*hash hashes the new words at corresponding spots with the help of 
 * other methods
 */
hashTable *hash(hashTable *tableOne, char *word, char* path){
	if(tableOne == NULL || word == NULL || path == NULL){
		return NULL;
	}
	
	int val;

	if(isdigit(word[0]) == 0){
		val = (-87) + word[0];
	}
	else{
		val = (-48) + word[0];	
	}
	if(tableOne->table[val] == NULL){
		tableOne->table[val] = createList(word,path);
	}else{
		tableOne->table[val] = addToList(word,tableOne->table[val],path);
	}
	return tableOne;
}

/*free list frees all the allocated memory for node or fileNode structs
*/
void* freeList(node *head){

	if(head == NULL){
		return NULL;
	}	
	node *temp = head;

	while(head != NULL){

		head = head->next;
		fileNode *tempFile = temp->fileList;
		while(tempFile != NULL){
			temp->fileList = temp->fileList->nextFile;
			free(tempFile->fileName);
			free(tempFile);
			tempFile = temp->fileList;
		}

		free(temp->word);
		free(temp);
		temp = head;
	}
	return NULL;
}
/*freeTable frees the allocated memory for the table after calling
 * freeList
 */
void* freeTable(hashTable *table){
	int i = 0;
	for(i = 0; i < 36; i++){
		freeList(table->table[i]);
	}
	free(table->table);
	free(table);

	return NULL;
}
/*fileParser goes through the received file, tokenizes the data and 
 * hashes all the tokens.
 */
void *fileParser(char *resultPath, char *path, hashTable *tableOne){
	FILE *fp;
	int n = 0;
	char *fArr;
	char c;
	fp = fopen(path,"r");
	fseek(fp, 0, SEEK_END);
	long f_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fArr = malloc(f_size);
	while((c = fgetc(fp)) != EOF){
		c = tolower(c);

		if(c < 48 || (c > 57 && c < 97) || c > 122){
			if(n > 0){
				fArr[n] = '\0';
				if(strlen(fArr) == 0){
					continue;
				}
				tableOne = hash(tableOne,fArr,path);
				
			}
			n = 0;
			continue;
		}
		fArr[n] = c;
		n++;
	}

	if(n > 0){
		tableOne = hash(tableOne,fArr,path);
	}
	free(fArr);
	fclose(fp);
	return NULL;
}
/*dirParser is a recursive function going through all the sub directories 
 * and sending every file it finds to the fileParser.
 */
void *dirParser(char *resultPath, char *path, hashTable *tableOne){
	struct stat s;
	if( stat(path,&s) == 0 )
	{
		if( s.st_mode & S_IFDIR )
		{
			DIR *dir;
			if(access(path,R_OK) == -1){
				printf("There are no read permissions to this folder %s, cannot proceed \n",path);
				return NULL;
			}

			dir = opendir(path);
			struct dirent *pDirent;

			char *namePtr;
			char newPath[1024];
			while((pDirent = readdir(dir)) != NULL){

				if(pDirent->d_name[0] == '.'){
					continue;
				}
				namePtr = pDirent->d_name;
				strcpy(newPath, path);
				strcat(newPath,"/");
				strcat(newPath,namePtr);
				int len = strlen(newPath);
				newPath[len] = '\0';
				if(access(newPath,R_OK) == -1){
					printf("There are no read permissions to this file %s the file is being skipped, other files will still be checked\n",newPath);
					continue;
				}
				dirParser(resultPath,newPath,tableOne);
				
			}
			closedir(dir);
		}
		else if( s.st_mode & S_IFREG )
		{	 if(access(path,R_OK) == -1){
							    printf("There are no read permissions to this file %s the file is being skipped, other files will still be checked\n",path);
							    return NULL;
						    }

		fileParser(resultPath,path,tableOne);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
	return NULL;
}
/*input inputs everything from the final hash table into the result file
*/  
void * input(char *path, hashTable *tableOne){

	FILE *result;
	result = fopen(path,"a");

	int i;
	node *temp;
	fileNode *tempFileNode;
	for(i = 0; i < 36; i++){
		if(tableOne->table[i] == NULL){
			continue;
		}
		temp = tableOne->table[i];
		while(temp != NULL){
			fputs("<list> ",result);
			fputs(temp->word,result);
			fputs("\n\n", result);
			tempFileNode = temp->fileList;
			while(tempFileNode != NULL){
				fprintf(result,"%s  %d\n",tempFileNode->fileName,tempFileNode->wordCount);

				fputs("\n",result);


				tempFileNode = tempFileNode->nextFile;
			}
			fputs("</list>\n",result);
			fputs("\n",result);

			temp = temp->next;
		}
	}
	fclose(result);
	freeTable(tableOne);
	return NULL;
}


