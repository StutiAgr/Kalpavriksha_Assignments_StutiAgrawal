#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100

typedef struct {
    int rollNumber;
    char name[50];
    int subject1, subject2, subject3;
    int totalMarks;
    float averageMarks;
    char grade;
} Student;

// Function to calculate total marks
int calculateTotal(int marks1, int marks2, int marks3) {
    return marks1 + marks2 + marks3;
}

// Function to calculate average marks
float calculateAverage(int total) {
    return total / 3.0;
}

// Function to assign grade
char assignGrade(float avg) {
    if(avg >= 85) return 'A';
    else if(avg >= 70) return 'B';
    else if(avg >= 50) return 'C';
    else if(avg >= 35) return 'D';
    else return 'F';
}

// Print performance pattern
void printPerformancePattern(char grade) {
    int stars;
    switch(grade) {
        case 'A':
            stars = 5;
            break;
        case 'B':
            stars = 4;
            break;
        case 'C':
            stars = 3;
            break;
        case 'D':
            stars = 2;
            break;
        default: // For 'F' or any invalid grade
            stars = 0;
            break;
    }
    for(int i = 0; i < stars; i++) {
        printf("*");
    }
}

// Print roll numbers recursively
void printRollNumbersRecursive(Student s[], int index, int n) {
    if(index==n) return;
    printf("%d ", s[index].rollNumber);
    printRollNumbersRecursive(s, index+1, n);
}

// Display student details
void displayStudentDetails(Student s) {
    printf("Roll: %d\n", s.rollNumber);
    printf("Name: %s\n", s.name);
    printf("Total: %d\n", s.totalMarks);
    printf("Average: %.2f\n", s.averageMarks);
    printf("Grade: %c\n", s.grade);
    if(s.grade != 'F') {
        printf("Performance: ");
        printPerformancePattern(s.grade);
        printf("\n");
    }
}

// Check if roll number is unique
int isRollNumberUnique(Student s[], int count, int roll) {
    for(int i=0;i<count;i++){
        if(s[i].rollNumber == roll) return 0; // not unique
    }
    return 1;
}

int main() {
    int N;
    Student students[MAX_STUDENTS];

    printf("Enter number of students: ");
    while(1){
        if(scanf("%d",&N)==1 && N>=1 && N<=MAX_STUDENTS){
            break;
        } else {
            printf("Invalid input! Enter number of students (1-100): ");
            while(getchar()!='\n'); // clear input
        }
    }

    getchar(); // consume newline after N

    for(int i=0;i<N;i++){
        char line[200];
        int valid = 0;
        while(!valid){
            printf("Enter student %d details (Roll Name Marks1 Marks2 Marks3): ", i+1);
            if(fgets(line,sizeof(line),stdin)==NULL){
                printf("Error reading input, try again.\n");
                continue;
            }

            // temporary variables to parse
            int roll, marks1,marks2,marks3;
            char name[50];

            int parsed = sscanf(line,"%d %s %d %d %d",&roll,name,&marks1,&marks2,&marks3);
            if(parsed !=5){
                printf("Invalid input format! Please type all details.\n");
                continue;
            }

            if(roll <=0){
                printf("Roll number must be positive. Re-enter details.\n");
                continue;
            }

            if(!isRollNumberUnique(students,i,roll)){
                printf("Roll number already exists! Re-enter details.\n");
                continue;
            }

            if(marks1<0||marks1>100 || marks2<0||marks2>100 || marks3<0||marks3>100){
                printf("Marks must be between 0 and 100! Re-enter details.\n");
                continue;
            }

            // If all validations pass, store student
            students[i].rollNumber = roll;
            strcpy(students[i].name,name);
            students[i].subject1 = marks1;
            students[i].subject2 = marks2;
            students[i].subject3 = marks3;

            students[i].totalMarks = calculateTotal(marks1,marks2,marks3);
            students[i].averageMarks = calculateAverage(students[i].totalMarks);
            students[i].grade = assignGrade(students[i].averageMarks);

            valid = 1;
        }
    }

    printf("\n");

    // Display all students
    for(int i=0;i<N;i++){
        displayStudentDetails(students[i]);
        if(i<N-1) printf("\n");
    }

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbersRecursive(students,0,N);
    printf("\n");

    return 0;
}
