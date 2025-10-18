#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int getMatrixSize(){
    int size;
    while(1){
        if(scanf("%d", &size) != 1){
            printf("Invalid input. Enter again: ");
            while(getchar() != '\n');
        }
        else if(size < 2 || size > 10){
            printf("Please enter number between 2-10: ");
        }
        else{
            return size;
        }
    }
}

void generateRandomMatrix(int** matrix, int size){
    srand(time(NULL));
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            *(*(matrix + row) + col) = rand() % 256;
        }
    }
}

void printMatrix(int** matrix, int size){
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            printf("%d ",*(*(matrix + row) + col));
        }
        printf("\n");
    }
}

void swapValues(int* value1, int* value2){
    int temp = *value1;
    *value1 = *value2;
    *value2 = temp;
}

void transposeMatrix(int** matrix, int size){
    for(int row = 0; row < size; row++){
        for(int col = row + 1; col < size; col++){
            swapValues((*(matrix + row) + col), (*(matrix + col) + row));
        }
    }
}

void mirrorMatrix(int** matrix, int size){
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size / 2; col++){
            swapValues((*(matrix + row) + col), (*(matrix + row) + (size - col - 1)));
        }
    }
}

void rotateMatrix(int** matrix, int size){
    transposeMatrix(matrix, size);
    mirrorMatrix(matrix, size);
}

void smoothMatrix(int** matrix, int size){
    int *prevRow = (int*)malloc(size * sizeof(int));
    int *currRow = (int*)malloc(size * sizeof(int));

    for(int matrixRow = 0; matrixRow < size; matrixRow++){
        for(int i = 0; i < size; i++){
            *(currRow + i) = *(*(matrix + matrixRow) + i);
        }

        for(int matrixCol = 0; matrixCol < size; matrixCol++){
            int sum = 0;
            int count = 0;

            for(int filterRow = matrixRow-1; filterRow <= matrixRow + 1; filterRow++){
                if(filterRow < 0 || filterRow >= size) continue;
                int *rowPtr;
                if(filterRow == matrixRow - 1) rowPtr = prevRow;
                else if(filterRow == matrixRow) rowPtr = currRow;
                else rowPtr = *(matrix + filterRow);

                for(int filterCol = matrixCol - 1; filterCol <= matrixCol + 1; filterCol++){
                    if(filterCol < 0 || filterCol >= size) continue;
                    sum += *(rowPtr + filterCol);
                    count++;
                }
            }

            *(*(matrix + matrixRow) + matrixCol) = sum / count;
        }
        for(int i = 0; i < size; i++){
            *(prevRow + i) = *(currRow + i);
        }
    }
    free(prevRow);
    free(currRow);
}

int main(){
    int size;
    printf("Enter matrix size (2-10): ");
    size = getMatrixSize();
    int** matrix = malloc(size * sizeof(int*));
    for(int row = 0; row < size; row++){
        *(matrix + row) = malloc(size * sizeof(int));
    }

    generateRandomMatrix(matrix, size);
    printf("Original Randomly Generated Matrix: \n");
    printMatrix(matrix, size);

    rotateMatrix(matrix, size);
    printf("Matrix after 90 degrees Clockwise Rotation: \n");
    printMatrix(matrix, size);

    smoothMatrix(matrix, size);
    printf("Matrix after Applying 3x3 Smoothing Filter: \n");
    printMatrix(matrix, size);
    
    for (int row = 0; row < size; row++) {
        free(*(matrix + row));
    }
    free(matrix);
}
