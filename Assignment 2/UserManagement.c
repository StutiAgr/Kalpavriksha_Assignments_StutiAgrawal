#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//file where records are stored
#define FILENAME "users.txt"

//structure to represent user
typedef struct{
    int id;
    char name[100];
    int age;
} User;

//functions declaration
int getNextId();
void createUser();
void readUsers();
void updateUser();
void deleteUser();
void menu();
int readInt(const char* prompt);

int main(){
    int choice;
    do{
        menu();
        printf("Enter choice: ");
        choice=readInt("Enter choice: ");
        switch(choice){
            case 1:
                createUser();
                break;
            case 2:
                readUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    }while(choice!=5);
    return 0;
}

//function to get next unique ID
//we scan file till the end and pick last id+1
int getNextId() {
    FILE *f=fopen(FILENAME, "r");
    int id=0,age;
    char name[100];
    if(!f) return 1; //if file doesn't exist, start ids from 1

    //read each line and overwrite id until end of file
    while(fscanf(f,"%d\t%99[^\t]\t%d", &id, name, &age)==3);
    fclose(f);
    return id+1;
}

void createUser(){
    FILE *f=fopen(FILENAME,"a");
    User u;
    if(f==NULL){
        printf("Error: Unable to open file");
        return;
    }
    printf("----Add New User----\n");
    //because id has to be unique, code will assign it itself
    u.id=getNextId();
    printf("Enter name: ");
    scanf(" %[^\n]", u.name);  //read full line until newline
    u.age=readInt("Enter age: ");
    fprintf(f,"%d\t%s\t%d\n",u.id,u.name,u.age);
    fclose(f);
    printf("User added. Assigned ID: %d\n",u.id);
}

void readUsers(){
    FILE *f=fopen(FILENAME,"r");
    if(!f){
        printf("Unable to open file.\n");
        return;
    }
    int count=0;
    User u;
    printf("%-5s %-25s %-5s\n","ID","Name","Age");
    while(fscanf(f,"%d\t%99[^\t]\t%d",&u.id,u.name,&u.age)==3){
        printf("%-5d %-25s %-5d\n",u.id,u.name,u.age);
        count++;
    }
    if(count==0) printf("No users found.\n");
    fclose(f);
}

//update details of user by searching with id
//copy everythiing to a temp file, only updating matching record
void updateUser(){
    int id,found=0;
    FILE *f=fopen(FILENAME,"r");
    FILE *temp=fopen("temp.txt","w");
    if(!f||!temp){
        printf("Unable to open file\n");
        return;
    }
    printf("\n----Update a User----\n");
    id=readInt("Enter ID to update: ");
    
    User u;
    while(fscanf(f,"%d\t%99[^\t]\t%d",&u.id,u.name,&u.age)==3){
        if(u.id==id){
            found=1;
            //ask for new values;
            printf("Enter new name: ");
            scanf(" %[^\n]",u.name);
            u.age=readInt("Enter new age: ");
        }
        fprintf(temp,"%d\t%s\t%d\n",u.id,u.name,u.age);
    }
    fclose(f);
    fclose(temp);
    //replace old file with new file
    remove(FILENAME);
    rename("temp.txt",FILENAME);

    if(found) printf("User updated.\n");
    else printf("User not found.\n");
}

//delete user by id
//copy all records except one with matching id
void deleteUser(){
    int id,found=0;
    FILE *f=fopen(FILENAME,"r");
    FILE *temp=fopen("temp.txt","w");
    if(!f||!temp){
        printf("Unable to open file.");
        return;
    }
    printf("\n----Delete a User----\n");
    id=readInt("Enter ID to delete: ");
    User u;
    while(fscanf(f,"%d\t%99[^\t]\t%d",&u.id,u.name,&u.age)==3){
        if(u.id==id){
            found=1;
            //skip writing this -> effectively deleting it
            continue;
        }
        fprintf(temp,"%d\t%s\t%d\n",u.id,u.name,u.age);
    }
    fclose(f);
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt",FILENAME);
    
    if(found) printf("User deleted.\n");
    else printf("User not found.\n");
}
void menu() {
    printf("\n----User Management----\n");
    printf("1. Create User\n");
    printf("2. Read Users\n");
    printf("3. Update User\n");
    printf("4. Delete User\n");
    printf("5. Exit\n");
}
//handling invalid inputs(other than integer)
int readInt(const char* prompt){
    int value;
    while(1){
        printf("%s",prompt);
        if(scanf("%d",&value)==1){
            while(getchar()!='\n'); //clear remaining buffer
            return value;
        }
        printf("Invalid input. Please enter a valid number.\n");
        while(getchar()!='\n');
    }
}