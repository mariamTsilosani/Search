#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include "index.h"
#include <unistd.h>


int main(int argc, char *argv[]){


	FILE *fp;
	if(argv[1] == NULL || argv[2] == NULL ){
		printf("no file/files provided\n");
		return 0;
	}

	int file_exist (char *filename)
	{
		struct stat   buffer;   
		return (stat (filename, &buffer) == 0);
	}

	int userInput;
	DIR *dir;
	char *result = argv[1];
	if (file_exist (result))
	{

		while(1){
			printf ("the privided file exists in the directory, please enter one of the choices: \n If you would like to append to the end of the existing file enter 1\n If you would like to overwrite the existing file enter 2\n If you would like to quit the program enter 3\n");
			scanf("%d", &userInput);



			if(userInput == 1){
				if((access(result,W_OK) == -1)||(access(result,R_OK) == -1)){
                                                printf("There are no read or write permissions to this file, cannot proceed\n");
                                                return 0;
                                        }

				if((dir = opendir(result)) != NULL){

					printf("The name you provided is a Directory name, thus it cannot have text appended to the end, please choose a different option\n");
					continue;
				}	

				break;
			}
			else if(userInput == 2){

				int status = remove(result);
				if(status != 0){
					printf("The file cannot be overwritten, please choose another option\n");
					continue;
				}

				fp = fopen(result,"ab");
				fclose(fp);
				break;
			}
			else if(userInput == 3){
				printf("Exiting the program!\n");
				return 0;

			}
			else{
				printf("Invalid entry\n");
				return 0;
			}

		}
	}
	char path[PATH_MAX+1];
	char pathTwo[PATH_MAX+1];
	char *ptr;
	ptr = realpath(result,path);
	char *ptrTwo;
	ptrTwo = realpath(argv[2],pathTwo);

	hashTable *tableOne = createTable(36);
	dirParser(ptr,ptrTwo,tableOne);
	input(path,tableOne);
	return 0;

}

