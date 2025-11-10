#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BLOCKS_PER_FILE 1024
#define LINE_LENGTH 1000
#define MAX_BLOCK_SIZE 512
#define MAX_NAME_LENGTH 51
#define MAX_NUM_BLOCKS 1024
#define PATH_SIZE 1000

typedef struct FreeBlock{
    int index;
    struct FreeBlock* prev;
    struct FreeBlock* next;
} FreeBlock;

typedef struct FileNode{
    char name[MAX_NAME_LENGTH];
    int isDirectory;
    struct FileNode* parent;
    struct FileNode* next;
    struct FileNode* child;
    int blockPointers[BLOCKS_PER_FILE];
    int contentSize;
    int numOfBlocks;
} FileNode;

char virtualDisk[MAX_NUM_BLOCKS][MAX_BLOCK_SIZE];
FreeBlock* freeListHead = NULL;
FileNode *root = NULL;
FileNode *cwd = NULL;

int isEqual(char* str1, char* str2){
    while(*str1 && *str2){
        if(*str1 != *str2) return 0;
        str1++;
        str2++;
    }
    return (*str1 == *str2);
}

int stringLength(char* str){
    int length = 0;
    while(*str){
        length++;
        str++;
    }
    return length;
}

void stringCopy(char* destination, char* source){
    while(*source){
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
}

void initializeFreeBlock(){
    FreeBlock* previous = NULL;
    for(int index = 0; index < MAX_NUM_BLOCKS; index++){
        FreeBlock* block = (FreeBlock*)malloc(sizeof(FreeBlock));
        block->index = index;
        block->next = NULL;
        block->prev = previous;
        if(previous){
            previous->next = block;
        }
        else{
            freeListHead = block;
        }
        previous = block;
    }
}

void initializeFileSystem(){
    initializeFreeBlock();
    root = (FileNode*)malloc(sizeof(FileNode));
    root->isDirectory = 1;
    root->parent = NULL;
    root->child = NULL;
    stringCopy(root->name,"/");
    root->next = NULL;
    root->contentSize = 0;
    root->numOfBlocks = 0;
    cwd = root;
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
}

int allocateFreeBlock(){
    if(freeListHead == NULL){
        printf("No memory left.\n");
        return -1;
    }
    FreeBlock* allocatedBlock = freeListHead;
    int index = allocatedBlock->index;
    freeListHead = freeListHead->next;
    if(freeListHead){
        freeListHead->prev = NULL;
    }
    free(allocatedBlock);
    return index;
}

void freeFileBlocks(FileNode* file){
    if(!file) return;
    for(int index = 0; index < file->numOfBlocks; index++){
        int blockIndex = file->blockPointers[index];
        if(blockIndex >= 0){
            FreeBlock* newFreeBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
            newFreeBlock->index = blockIndex;
            newFreeBlock->next = freeListHead;
            if(freeListHead){
                freeListHead->prev = newFreeBlock;
            }
            newFreeBlock->prev = NULL;
            freeListHead = newFreeBlock;
            file->blockPointers[index] = -1;
        }
    }
    file->contentSize = 0;
    file->numOfBlocks = 0;
}

void mkdir(char* name){
    FileNode* node = cwd->child;
    if(node){
        do{
            if(isEqual(node->name, name)){
                printf("Name already exists in current directory.\n");
                return;
            }
            node = node->next;
        }while(node != cwd->child);
    }
    FileNode* newDirectory = (FileNode*)malloc(sizeof(FileNode));
    stringCopy(newDirectory->name, name);
    newDirectory->isDirectory = 1;
    newDirectory->parent = cwd;
    newDirectory->child = NULL;
    newDirectory->contentSize = 0;
    newDirectory->numOfBlocks = 0;
    if(cwd->child == NULL){
        cwd->child = newDirectory;
        newDirectory->next = newDirectory;
    }
    else{
        FileNode* temp = cwd->child;
        while(temp->next != cwd->child){
            temp = temp->next;
        }
        newDirectory->next = temp->next;
        temp->next = newDirectory;
    }
    printf("Directory '%s' created successfully.\n", name);
}

void ls(){
    FileNode* temp = cwd->child;
    if(temp == NULL){
        printf("[empty]\n");
        return;
    }
    do{
        printf("%s",temp->name);
        if(temp->isDirectory){
            printf("/");
        }
        printf("\n");
        temp = temp->next;
    }while(temp != cwd->child);
}

void pwd(){
    FileNode* temp = cwd;
    char path[PATH_SIZE] = "";
    char tempPath[PATH_SIZE] = "";
    path[0] = '\0';
    tempPath[0] = '\0';
    while(temp != root){
        sprintf(tempPath,"%s/%s",temp->name, path);
        stringCopy(path, tempPath);
        temp = temp->parent;
    }
    if(stringLength(path) == 0) printf("/\n");
    else{
        path[stringLength(path) - 1] = '\0';
        printf("/%s\n", path);
    } 
}

void cdParent(){
    if(cwd == root){
        printf("Already on root.\n");
        return;
    }
    cwd = cwd->parent;
    printf("Moved to ");
    pwd();
}

void cd(char *name){
    if(isEqual(name, "..")){
        cdParent();
    }
    else{
        if(cwd->child == NULL){
            printf("%s is empty.\n",cwd->name);
            return;
        }
        FileNode* temp = cwd->child;
        do{
            if(isEqual(temp->name, name)){
                if(temp->isDirectory == 1){
                    cwd = temp;
                    printf("Moved to ");
                    pwd();
                    return;
                }
            }
            temp = temp->next;
        }while(temp != cwd->child);
        printf("Directory not found.");
    }
}

void create(char* name){
    FileNode* node = cwd->child;
    if(node){
        do{
            if(isEqual(node->name, name)){
                printf("Name already exists in current directory.\n");
                return;
            }
            node = node->next;
        }while(node != cwd->child);
    }
    FileNode* newFile = (FileNode*)malloc(sizeof(FileNode));
    stringCopy(newFile->name, name);
    newFile->isDirectory = 0;
    newFile->parent = cwd;
    newFile->child = NULL;
    newFile->contentSize = 0;
    newFile->numOfBlocks = 0;
    for(int i = 0; i < BLOCKS_PER_FILE; i++){
        *(newFile->blockPointers + i) = -1;
    }
    if(cwd->child == NULL){
        cwd->child = newFile;
        newFile->next = newFile;
    }
    else{
        FileNode* temp = cwd->child;
        while(temp->next != cwd->child){
            temp = temp->next;
        }
        newFile->next = temp->next;
        temp->next = newFile;
    }
    printf("File '%s' created successfully.\n", name);
}

void writeData(FileNode* file, char* data){
    if(!file || file->isDirectory){
        printf("Invalid file.\n");
        return;
    }
    int lastBlockIndex = -1;
    int remainingSpace = 0;
    int dataLength = stringLength(data);
    if(file->numOfBlocks > 0){
        lastBlockIndex = file->blockPointers[file->numOfBlocks - 1];
        remainingSpace = MAX_BLOCK_SIZE - (file->contentSize % MAX_BLOCK_SIZE);
    }
    int written = 0;
    if(remainingSpace > 0 && lastBlockIndex != -1){
        int copySize = (dataLength < remainingSpace) ? dataLength : remainingSpace;
        strncpy(virtualDisk[lastBlockIndex] + (MAX_BLOCK_SIZE - remainingSpace), data, copySize);
        written += copySize;
    }
    while(written < dataLength){
        int blockIndex = allocateFreeBlock();
        if(blockIndex == -1) return;
        file->blockPointers[file->numOfBlocks] = blockIndex;
        file->numOfBlocks++;
        int copySize = (dataLength - written > MAX_BLOCK_SIZE) ? MAX_BLOCK_SIZE : (dataLength - written);
        strncpy(virtualDisk[blockIndex], data + written, copySize);
        written += copySize;
    }
    file->contentSize += dataLength;
    printf("Data written successfully (size = %d bytes)\n", dataLength);
}

void write(char* filename, char* data){
    if(data[0] != '"' || data[stringLength(data) - 1] != '"'){
        printf("Syntax error.\n");
        return;
    }
    data[stringLength(data) - 1] = '\0';
    data++;
    int j = 0;
    for(int i = 0; data[i] != '\0'; i++, j++){
        if(data[i] == '\\' && data[i+1] == 'n'){
            data[j] = '\n';
            i++;
        }
        else{
            data[j] = data[i];
        }
    }
    data[j] = '\0';

    FileNode *temp = cwd->child;
    if(temp == NULL){
        printf("No file found.\n");
        return;
    }
    do{
        if(isEqual(temp->name, filename)){
            writeData(temp, data);
            return;
        }
        temp = temp->next;
    }while(temp != cwd->child);
    printf("File not found.\n", filename);
}

void readData(FileNode* file){
    if(file->numOfBlocks == 0){
        printf("[empty]\n");
        return;
    }
    for(int index = 0; index < file->numOfBlocks; index++){
        printf("%s",virtualDisk[file->blockPointers[index]]);
    }
    printf("\n");
}

void read(char* name){
    FileNode* temp = cwd->child;
    if(!temp){
        printf("No file found.\n");
        return;
    }
    do{
        if(isEqual(temp->name, name)){
            if(!temp->isDirectory){
                readData(temp);
                return;
            }
        }
        temp = temp->next;
    }while(temp != cwd->child);
    printf("No file found.\n");
}

void deleteFile(FileNode* file){
    freeFileBlocks(file);
    if(file->next == file){
        cwd->child = NULL;
    }
    else{
        FileNode* temp = cwd->child;
        while(temp->next != file && temp->next != cwd->child){
            temp = temp->next;
        }
        temp->next = file->next;
        if(file == cwd->child){
            cwd->child = temp->next;
        }
    }
    free(file);
    printf("File deleted successfully.\n");
}

void delete(char* name){
    if(cwd->child == NULL){
        printf("No file found.\n");
        return;
    }
    FileNode* temp = cwd->child;
    do{
        if(isEqual(temp->name, name)){
            if(!temp->isDirectory){
                deleteFile(temp);
                return;
            }
        }
        temp = temp->next;
    }while(temp != cwd->child);
    printf("No file found.\n");
}

void rmdir(char* name){
    if(cwd->child == NULL){
        printf("No directory found.\n");
        return;
    }
    FileNode* temp = cwd->child;
    do{
        if(isEqual(temp->name, name)){
            if(!temp->isDirectory){
                printf("%s is a file. Use delete command.", name);
                return;
            }
            if(temp->child != NULL){
                printf("Directory not empty. Remove files first.\n");
                return;
            }
            if(temp->next == temp){
                cwd->child = NULL;
            }
            else{
                FileNode* node = cwd->child;
                while(node->next != temp && node->next != cwd->child){
                    node = node->next;
                }
                node->next = temp->next;
                if(temp == cwd->child){
                    cwd->child = node->next;
                }
            }
            free(temp);
            printf("Directory removed successfully.\n");
            return;
        }
        temp = temp->next;
    }while(temp != cwd->child);
    printf("No directory found.\n");
}

void df(){
    int freeBlocks = 0;
    FreeBlock* temp = freeListHead;
    while(temp != NULL){
        freeBlocks++;
        temp = temp->next;
    }
    printf("Total Blocks: %d\n", MAX_NUM_BLOCKS);
    printf("Used Blocks: %d\n", MAX_NUM_BLOCKS - freeBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", (float) ((MAX_NUM_BLOCKS - freeBlocks) * 100) / MAX_NUM_BLOCKS);
}

void freeFreeBlocks(){
    FreeBlock* temp = freeListHead;
    while(temp){
        FreeBlock* next = temp->next;
        free(temp);
        temp = next;
    }
    freeListHead = NULL;
}

void freeFileNodes(FileNode* node){
    if(!node) return;
    FileNode* child = node->child;
    if(child){
        FileNode* start = child;
        FileNode* last = child;

        while(last->next != start){
            last = last->next;
        }
        last->next = NULL;
        FileNode* current = start;
        while(current){
            FileNode* nextChild = current->next;
            freeFileNodes(current);
            current = nextChild;
        }
    }
    if(!node->isDirectory){
        freeFileBlocks(node);
    }
    free(node);
}

void exit_system(){
    freeFreeBlocks();
    freeFileNodes(root);
    printf("Memory released. Exiting program...\n");
}

void takeInput(){
    printf("\n%s> ",cwd->name);
    char line[LINE_LENGTH];
    fgets(line, LINE_LENGTH, stdin);
    line[strcspn(line, "\n")] = '\0';

    char* command = strtok(line, " ");
    if(command == NULL) return;
    
    if(isEqual(command, "mkdir")){
        char* argument = strtok(NULL, " ");
        if(argument == NULL){
            printf("Syntax: mkdir directory_name\n");
            return;
        }
        mkdir(argument);
    }
    else if(isEqual(command, "cd")){
        char* argument = strtok(NULL, " ");
        if(argument == NULL){
            printf("Syntax: cd .. OR cd directory_name\n");
            return;
        }
        cd(argument);
    }
    else if(isEqual(command, "cd..")){
        cdParent();
    }
    else if(isEqual(command, "ls")){
        ls();
    }
    else if(isEqual(command, "create")){
        char* argument = strtok(NULL, " ");
        if(argument == NULL){
            printf("Syntax: create file_name\n");
            return;
        }
        create(argument);
    }
    else if(isEqual(command, "write")){
        char* filename = strtok(NULL, " ");
        char* data = strtok(NULL,"\n");
        if(!filename || !data){
            printf("Syntax: write filename \"data\"");
            return;
        }
        write(filename, data);
    }
    else if(isEqual(command, "read")){
        char* argument = strtok(NULL, " ");
        if(argument == NULL){
            printf("Syntax: read file_name\n");
            return;
        }
        read(argument);
    }
    else if(isEqual(command, "delete")){
        char* argument = strtok(NULL, " ");
        if(argument == NULL){
            printf("Syntax: delete filename\n");
        }
        delete(argument);
    }
    else if(isEqual(command, "rmdir")){
        char* argument = strtok(NULL," ");
        if(argument == NULL){
            printf("Syntax: rmdir filename\n");
        }
        rmdir(argument);
    }
    else if(isEqual(command,"pwd")){
        pwd();
    }
    else if(isEqual(command, "df")){
        df();
    }
    else if(isEqual(command, "exit")){
        exit_system();
        exit(0);
    }
    else{
        printf("Invalid command\n");
    }
}

int main(){
    initializeFileSystem();
    while(1){
        takeInput();
    }
    return 0;
}
