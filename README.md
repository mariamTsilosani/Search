# Search

#####This search program is closely related to the Indexer program that's in a different repo. This program loads an inverted index produced by the Indexer program into memory and uses it to answer users' search queries. 

#####There are 7 different commands users can enter:
    1. sa: Performs a "String AND", telling you what files contain ALL of the searched terms.
    2. so: Performs a "String OR", telling you what files contain ANY of the searched terms.
    3. na: Performs a "String NAND", telling you what files do not contain All of the searched terms.
    4. no: Performs a "String NOR", telling you what files do not contain ANY of the searched terms.
    5. fi: Lists all the file names in the provided input file.
    6. words: Lists all the words(tokens) found in the provided input file.
    7. q: Quits the search program.

###Implementation

#####The implementation of the search function is a two-dimensional linked list. The index file provided is read in, parsed and saved in the linked list, called 'list', with words(tokens) first and then all the file names that contain that word.

#####This structure was used to make a new structure, called 'result' containing file names and the words being searched that were found in that specific file. 'result' helped derive 'sa' by printing only the file names containing all the words, 'so' by printing everything contained in that structure and 'na' by printing the files that did NOT contain all the words searched. there also is a smaller linked list, called 'fileList', containing all the files that exist in the index file.

#####'fileList' helped with the implementation of 'na' and 'no' by getting all the files that do not contain any words searched and it also made 'fi' by printing out the whole fileList. While 'words' is printing out the words without the filenames they were found in.

##### all the structures get freed before the user inputs the next command, only 'list' and 'fileList' wait to be freed until the user chooses to quit the program since those structures are reused for every command entered.

##### The program also deals with duplicate entries, if the same word was entered twice in the search, the program will only search for it once.

### Exception Handling:

####This program handles the following cases:

    1. It is not given the appropriate number of command line arguments at launch.
    2. It makes sure the input file exists.
    3. It makes sure that there is at least a read access to the input file.
    4. It makes sure the input file isn't empty.
    5. It makes sure the input file was produced by the Indexer program.
    6. If the output of one of the commands is blank, search leaves you a small prompt informing you that there are no files that fit your search criteria.
