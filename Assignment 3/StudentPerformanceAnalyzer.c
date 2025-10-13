#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100

typedef struct {
    int rollNumber;
    char name[50];
    int subject1, subject2, subject3;
} Student;

char assignGrade(float avg) {
    if (avg >= 85) return 'A';
    else if (avg >= 70) return 'B';
    else if (avg >= 50) return 'C';
    else if (avg >= 35) return 'D';
    else return 'F';
}

void printPerformancePattern(char grade) {
    int stars = 0;
    switch (grade) {
        case 'A': stars = 5; break;
        case 'B': stars = 4; break;
        case 'C': stars = 3; break;
        case 'D': stars = 2; break;
        default: stars = 0; break;
    }
    for (int i = 0; i < stars; i++) printf("*");
}

void printRollNumbersRecursive(Student students[], int index, int count) {
    if (index == count) return;
    printf("%d ", students[index].rollNumber);
    printRollNumbersRecursive(students, index + 1, count);
}

int isRollNumberUnique(Student students[], int count, int roll) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollNumber == roll) return 0;
    }
    return 1;
}

void displayStudentDetails(Student s) {
    int total = s.subject1 + s.subject2 + s.subject3;
    float avg = total / 3.0f;
    char grade = assignGrade(avg);

    printf("Roll: %d\n", s.rollNumber);
    printf("Name: %s\n", s.name);
    printf("Total: %d\n", total);
    printf("Average: %.2f\n", avg);
    printf("Grade: %c\n", grade);
    if (grade != 'F') {
        printf("Performance: ");
        printPerformancePattern(grade);
        printf("\n");
    }
}

int inputNumberOfStudents() {
    int count;
    printf("Enter number of students: ");
    while (1) {
        if (scanf("%d", &count) == 1 && count >= 1 && count <= MAX_STUDENTS)
            return count;
        else {
            printf("Invalid input! Enter number of students (1-100): ");
            while (getchar() != '\n');
        }
    }
}

Student inputSingleStudent(Student students[], int index) {
    Student s;
    int marks1, marks2, marks3;

    printf("\n--- Enter details for Student %d ---\n", index + 1);
    while (1) {
        printf("Roll number: ");
        if (scanf("%d", &s.rollNumber) == 1 && s.rollNumber > 0) {
            if (isRollNumberUnique(students, index, s.rollNumber)) break;
            else printf("Roll number already exists! Re-enter.\n");
        } else {
            printf("Invalid roll number! Enter again.\n");
            while (getchar() != '\n');
        }
    }
    printf("Name: ");
    scanf(" %[^\n]", s.name);
    while (1) {
        printf("Marks for Subject 1: ");
        if (scanf("%d", &marks1) == 1 && marks1 >= 0 && marks1 <= 100) break;
        else { printf("Invalid marks! Enter again (0-100).\n"); while (getchar() != '\n'); }
    }
    while (1) {
        printf("Marks for Subject 2: ");
        if (scanf("%d", &marks2) == 1 && marks2 >= 0 && marks2 <= 100) break;
        else { printf("Invalid marks! Enter again (0-100).\n"); while (getchar() != '\n'); }
    }
    while (1) {
        printf("Marks for Subject 3: ");
        if (scanf("%d", &marks3) == 1 && marks3 >= 0 && marks3 <= 100) break;
        else { printf("Invalid marks! Enter again (0-100).\n"); while (getchar() != '\n'); }
    }

    s.subject1 = marks1;
    s.subject2 = marks2;
    s.subject3 = marks3;
    return s;
}

void inputAllStudents(Student students[], int *count) {
    *count = inputNumberOfStudents();

    for (int i = 0; i < *count; i++) {
        students[i] = inputSingleStudent(students, i);
    }
}

void displayAllStudents(Student students[], int count) {
    printf("\n----- STUDENT DETAILS -----\n");
    for (int i = 0; i < count; i++) {
        displayStudentDetails(students[i]);
        if (i < count - 1) printf("\n");
    }

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbersRecursive(students, 0, count);
    printf("\n");
}

int main() {
    Student students[MAX_STUDENTS];
    int count;

    inputAllStudents(students, &count);
    displayAllStudents(students, count);

    return 0;
}
