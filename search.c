#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
/*The structs used for most of the implementation of this program*/
typedef struct node{
	char *word;
	struct node *next;
	struct fileNode *fileList;
}node;
typedef struct fileNode{
	char *fileName;
	struct fileNode *nextFile;
}fileNode;

/*build builds the structure containing the file names and words (searched words)*/

node *build(char* token, node* result, node* list){
	if(list == NULL){
		return NULL;
	}
	node* curr;
	curr = list;
	fileNode* currFile;
	node* new;
	fileNode* newFile;
	while(curr != NULL){
		if(strcmp(curr->word,token) == 0){
			currFile = curr->fileList;
			while(currFile != NULL){
				if(result == NULL){
					new = malloc(sizeof(node));
					new->word = malloc(strlen(currFile->fileName) + 1);
					strcpy(new->word,currFile->fileName);
					newFile = malloc(sizeof(fileNode));
					newFile->fileName = malloc(strlen(token) + 1);
					strcpy(newFile->fileName,token);
					newFile->nextFile = NULL;
					new->next = NULL;
					new->fileList = newFile;
					result = new;
					currFile = currFile->nextFile;	
				}
				else{
					node* search;
					fileNode* existFile;
					search = result;
					while(search != NULL){
					if(strcmp(search->word,currFile->fileName) == 0){
						existFile = malloc(sizeof(fileNode));
					      existFile->fileName = malloc(strlen(token) + 1);
						strcpy(existFile->fileName,token);
						existFile->nextFile = search->fileList;
							search->fileList = existFile;
							break;
						}
						search = search->next;
					}
					if(search != NULL){
						currFile = currFile->nextFile;
						continue;
					}
					new = malloc(sizeof(node));
					new->word = malloc(strlen(currFile->fileName) + 1);
					newFile = malloc(sizeof(fileNode));
					strcpy(new->word,currFile->fileName);
					newFile->fileName = malloc(strlen(token) + 1);	
					strcpy(newFile->fileName,token);
					new->fileList = newFile;
					newFile->nextFile = NULL;
					new->next = result;
					result = new;	
					currFile = currFile->nextFile;
				}
			}
		}
		curr = curr->next;
	}
	return result;
}

/*setLL reads in the input file and divides all the information into a 2D linked list*/ 

node* setLL(FILE *fp, int fileSize){
	char* buffer;
	int i;
	node* head;
	head = NULL;
	buffer = malloc(fileSize);
	while(1){
		for(i = 0; i < 6; i++){
			buffer[i] = fgetc(fp);
			if(buffer[i] == '\n'){
				i--;
				continue;
			}
			if(buffer[i] == EOF){
				free(buffer);
				return head;
			}
		}
		buffer[6] = '\0';
		if(strcmp(buffer,"<list>") != 0){
			printf("invalid input file\n");
			free(buffer);
			return NULL;
		}
		i = 0;
		while(1){
			buffer[i] = fgetc(fp);
			if(buffer[i] == ' '){
				i--;
			}
			if(buffer[i] == '\n' || buffer[i] == EOF){
				buffer[i] = '\0';
				break;
			}
			i++;
		}
		if(strlen(buffer) == 0){
			printf("invalid input file\n");
			free(buffer);
			return NULL;
		}
		node* new;
		new = malloc(sizeof(node));
		new->word = malloc((strlen(buffer) + 1) * sizeof(char));
		new->fileList = NULL;
		strcpy(new->word,buffer);
		if(head == NULL){
			head = new;
		} 
		else{
			new->next = head;
			head = new;
		}
		i = 0;
		int k = 0;
		while(1){
			i = 0;
			k = 0;
			while(1){
				buffer[i] = fgetc(fp);
				if(buffer[i] == '\n'){
					buffer[i] = '\0';
					break;
				} 
				if(buffer[i] == EOF){
					free(buffer);
					return head;
				}
				i++;
				if(buffer[i] == ' '){
					k++;	
				}
			}
			if(k == i){
				continue;
			}
			while(1){
				i--;
				if(buffer[i] == ' '){
					buffer[i-1] = '\0';
					break;
				}
				if(i == 0){
					break;
				}					
			}
			if(strcmp(buffer,"</list>") == 0){
				break;
			}
			fileNode *file = malloc(sizeof(fileNode));
			file->fileName = malloc(strlen(buffer) + 1);
			strcpy(file->fileName,buffer);
			file->nextFile = head->fileList;
			head->fileList = file;
			i = 0;
			continue;
		}
		continue;
	}
}

/*searchNode determines if a node already exists in the list provided*/

int searchNode(node* head,char* target){
	if(head == NULL){
		return 0;	
	}
	if(target == NULL){
		printf("Target was NULL in search node\n");
		return 0;	
	}
	node * curr;
	curr = head;
	while(curr){
		if(strcmp(curr->word,target) == 0){
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}

/*fullList makes a list of all the file names in the input file*/
 
node *fullList(node* list){
	node* curr= list;
	node* files = NULL;
	int check;
	while(curr){
		fileNode *cList;
		cList = curr->fileList;
		while(cList){	
			if(files){
			check = searchNode(files,cList->fileName);
			}
			else check = 0;
			if(check == 1 ){
				cList = cList->nextFile;
				continue;
			}
			node *new = malloc(sizeof(node));
			new->word = malloc((strlen(cList->fileName ) + 1) * sizeof(char));
			new->fileList = NULL; 
			strcpy(new->word,cList->fileName);
			new->next = files;
			files = new;
			cList = cList->nextFile;	
		}
		curr = curr->next;
	}
	return files; 
}

/*no prints out all the files that didn't contain any searched words*/

int no(node* result, node* files){
	node *file;
	node *curr;
	file = files;
	curr = result;
	int exist = 0;
	while(file){
		int check = searchNode(result,file->word);
		if(check == 0){
			exist = 1;
			printf("\x1b[45m%s\x1b[0m\n",file->word);
		}
		file = file->next;
	}
	if(exist == 0){
		return 0;
	}
	return 1;
}

/*print prints the list provided to the function*/

void* print(node* printList){
	node * curr;
	fileNode* currFile;
	curr = printList;
	while(curr != NULL){
		printf("\x1b[45m%s\x1b[0m",curr->word);
		currFile = curr->fileList;
		if(currFile != NULL){
			printf(" Contains: ");
		}	
		while(currFile != NULL){
			printf("\x1b[46m'%s'\x1b[0m",currFile->fileName);
			currFile = currFile->nextFile;
		}
		curr = curr->next;
		printf("\n");
	}
	return NULL;
}

/*freeList frees all the dynamically allocated memory in the list provided to the function, this method is called several times from main*/
 
void* freeList(node *head){

        if(head == NULL){
                return NULL;
        }
        node *temp;
	temp = head;

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

/*main takes care of the user interaction*/

int main(int argc, char *argv[]){
	if(argc != 2){
		printf( "\x1b[32mSearch program must have 2 arguments.\x1b[0m \n");		
		printf("\x1b[45mSearch Program Exited Due to Invalid Input\x1b[0m\n");
		return -1;
	}
	if(fopen(argv[1],"r") == NULL){
		printf("\x1b[32mFile does not exist.\x1b[0m \n");
		printf("\x1b[45mSearch Program Exited Due to Invalid Input\x1b[0m\n");
		return -1;
	} 
	if(access(argv[1],R_OK) == -1){
		printf("\x1b[32mProgram does not have read access to the file.\x1b[0m \n");
		printf("\x1b[45mSearch Program Exited Due to Invalid Input\x1b[0m\n");
		return -1;
	}

	DIR* dir = opendir(argv[1]);
	if(dir != NULL){
		printf("\x1b[32mThe input you provided is a directory.\x1b[0m\n");
		printf("\x1b[45mSearch Program Exited Due to Invalid Input\x1b[0m\n");
		return -1;
	}
	FILE* fp = fopen(argv[1],"r");
	fseek(fp, 0, SEEK_END);
	long f_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fclose(fp);

	if(f_size < 1){
		printf("\x1b[32mInput file is empty\x1b[0m \n");
		printf("\x1b[45mSearch Program Exited Due to Invalid Input\x1b[0m\n");
		return -1;
	}

	fp=fopen(argv[1] , "r"); 
	node* list;
	list = setLL(fp,f_size);
	fclose(fp);

	node* fileList;
	fileList = fullList(list);
	
	if(list == NULL){
		printf("\x1b[32mPlease rerun the program and pass the file produced by the indexer we provided you with.\x1b[0m \n");
		printf("\x1b[45mSearch Program Exited Due to Invalid Input\x1b[0m\n");
		return -1;
	}
	printf("file loaded successfully \n");
	while(1){
		char* choice = 0;                                  
		printf("Please Make a Selection: \n");
		
		printf("\x1b[34m'sa' Performs a 'String AND', telling you what files contain ALL of the terms.\x1b[0m \n");
		
		printf("\x1b[32m'so' Performs a 'String OR', telling you what files contain ANY of the terms.\x1b[0m \n");
		
		printf("\x1b[34m'na' Performs a 'String NAND',  telling you what files do not contain ALL of the terms.\x1b[0m \n");
		
		printf("\x1b[32m'no' Performs a 'String NOR', telling you what files do not contain ANY of the terms.\x1b[0m \n"); 
				printf("\x1b[34m'fi' Lists all the file names in the provided input file, if you input 'fi word word..' the words will be ignored and only all the file names will be printed out.\x1b[0m \n");

		printf("\x1b[32m'words' lists all the words(tokens) found in the provided input file.\x1b[0m\n");
	
		printf("\x1b[33m'q' Quits the search program.\x1b[0m \n");
		
		getline(&choice,malloc(sizeof(char)),stdin);
		if(choice==NULL||choice[0]=='\0'||choice[0] == '\n'){ 
			printf("please enter an option \n"); 
			continue;
		}
		char *token;
		int len = strlen(choice);
		choice[len - 1] = '\0';
		token = strtok(choice," ");

		node *result;
		result = NULL;
		node *curr;
		fileNode *currFile;
		result = NULL;
		node *wordList;
		wordList = NULL;
		node *newWord;

		char* command = malloc(strlen(token) + 1);
		strcpy(command,token);
		token = strtok(NULL," ");
		int count = 0;
		while(token != NULL){
			if(wordList != NULL){
				curr = wordList;
				int boolean = 0;
				while(curr != NULL){
					if(strcmp(curr->word,token)==0){
						token = strtok(NULL," ");
						boolean = 1;
						break;
					}
				curr = curr->next;	
				}
				if(boolean == 1){
					continue;
				}
			}
			count++;
			newWord = malloc(sizeof(node));
			newWord->word = malloc(strlen(token) + 1);
			strcpy(newWord->word,token);
			newWord->fileList = NULL;
			newWord->next = wordList;
			wordList = newWord;
			token = strtok(NULL," ");
		}
		newWord = wordList;
		if(strcmp(command,"sa")==0 ){ 
			newWord = wordList;
			while(newWord != NULL){
				result = build(newWord->word,result,list);
				newWord = newWord->next;
			}
			if(result == NULL){
				printf("no files found\n");
			}
			newWord = result;
			int boolean = 0;
			while(newWord != NULL){
				int wordCount = 0;
				currFile = newWord->fileList;
				while(currFile != NULL){
					wordCount++;
					currFile = currFile->nextFile;
				}
				if(wordCount == count){
					boolean = 1;	
					printf("\x1b[45m%s\x1b[0m",newWord->word);
					currFile = newWord->fileList;
					printf(" Contains: ");
					while(currFile != NULL){
						printf("\x1b[46m'%s'\x1b[0m",currFile->fileName);
						currFile = currFile->nextFile;
					}
					printf("\n");
				}
				newWord = newWord->next;
			}
			if(boolean == 0){
				printf("\x1b[45mThere are no files to display\x1b[0m\n");
			}
			freeList(result);
			free(choice);
			freeList(wordList);
			free(command);
			result = NULL;
		}
		else if(strcmp(command,"so")==0 ){ 
			while(newWord != NULL){
				result = build(newWord->word,result,list); 
				newWord = newWord->next;
			}
			if(result == NULL){
				printf("\x1b[45mNo files contain any of the given words\x1b[0m\n");
				continue;
			}
			curr = result;
			print(result);

			freeList(result);
			free(choice);
			freeList(wordList);
			free(command);
			result = NULL;
		}
		else if(strcmp(command,"na")==0 ){
			newWord = wordList;
			while(newWord != NULL){
				result = build(newWord->word,result,list);
				newWord = newWord->next;
			}
			newWord = result;
			int boolean = 0;
			while(newWord != NULL){
				int wordCount = 0;
				currFile = newWord->fileList;
				while(currFile != NULL){
					wordCount++;
					currFile = currFile->nextFile;
				}
				if(wordCount < count){
					boolean = 1;
					printf("\x1b[45m%s\x1b[0m Contains: ",newWord->word);
					currFile = newWord->fileList;
					while(currFile != NULL){
						printf("\x1b[46m'%s'\x1b[0m",currFile->fileName);
						currFile = currFile->nextFile;
					}
					printf("\n");
				}		
				newWord = newWord->next;
			}
			int exist;
			exist = no(result,fileList);
                        if(exist == 0 && boolean == 1){
                                printf("\x1b[45mThere are no files to display.\x1b[0m\n");
                        }
			freeList(result);
			free(choice);
			freeList(wordList);
			free(command);
			result = NULL;
		}
		else if(strcmp(command,"no")==0 ){  
			newWord = wordList;
			while(newWord != NULL){
				result = build(newWord->word,result,list);
				newWord = newWord->next;
			}
			int exist;
			exist = no(result,fileList);
			if(exist == 0){
                		printf("\x1b[45mThere are no files that don't contain any of the given words\x1b[0m\n");
        		}
			freeList(result);
			free(choice);
			freeList(wordList);
			free(command);
			result = NULL;

		}
		else if(strcmp(command,"fi")==0){
			node* curr = fileList;
			while(curr!=NULL){
				printf("\x1b[45m%s\x1b[0m\n",curr->word);
				curr = curr->next;	
			}
		}
		else if(strcmp(command,"words")==0){
			node* curr = list;
			while(curr != NULL){
				printf("\x1b[45m%s\x1b[0m\n",curr->word);
				curr = curr->next;
			}
		}
		else if(strcmp(command,"q")==0 ){  
			printf("\x1b[45mSearch Program Exited\x1b[0m\n");

			free(choice);
			freeList(fileList);
			freeList(list);
			freeList(wordList);
			free(command);

			break;
		}
		else if(strcmp(command,"ta")==0){
			printf("\x1b[45m214 TAs are AWESOME!!!!!\x1b[0m\n");
		}
		else{
			printf("\x1b[45mPlease enter a valid command.\x1b[0m\n"); 

		}
	}
	return 0; 
}
