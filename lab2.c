#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab2.h"


int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    int** board = NULL;

    int i=0, j=0, num, numrows = 9, numcols = 9, ch;

    for (i=0; i < numrows; ++i) {
        for (j=0; j < numcols; ++j) {
            while (((ch = getc(fp)) != EOF) && (ch == ',')) 
            if (ch == EOF) break;                           
            ungetc(ch, fp);
            if (fscanf("%d", &num) != 1) break;
            board[i][j] = num;                              
        }
    }

    return board;
}


int is_board_valid(){
    void *isColumnValid(void* param) {
        parameters *params = (parameters*) param;
        int row = params->row;
        int col = params->column;

        if (row != 0 || col > 8) {
            fprintf(stderr, "Invalid row or column for col subsection! row=%d, col=%d\n", row, col);
            pthread_exit(NULL);
        }
        int validityArray[9] = {0};
        int i;

        for (i = 0; i < 9; i++) {
            int num = sudoku[i][col];
        
            if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
                pthread_exit(NULL);
            } 
            else {
            validityArray[num - 1] = 1;
            }
        }

        valid[18 + col] = 1;
        pthread_exit(NULL);
    }

    void *isRowValid(void* param) {
        parameters *params = (parameters*) param;
        int row = params->row;
        int col = params->column;
        
        int valid[];

        if (col != 0 || row > 8) {
            fprintf(stderr, "Invalid row or column for row subsection! row=%d, col=%d\n", row, col);
            pthread_exit(NULL);
        }
        int validityArray[9] = {0};
        int i;

        for (i = 0; i < 9; i++) {
            int num = sudoku[row][i];
            if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
                pthread_exit(NULL);
            } 
            else {
                validityArray[num - 1] = 1;
            }
        }
        valid[9 + row] = 1;
        pthread_exit(NULL);
    }


    void *is3x3Valid(void* param) {
        parameters *params = (parameters*) param;
        int row = params->row;
        int col = params->column;
        int valid[];
        if (row > 6 || row % 3 != 0 || col > 6 || col % 3 != 0) {
            fprintf(stderr, "Invalid row or column for subsection! row=%d, col=%d\n", row, col);
            pthread_exit(NULL);
            }

        int validityArray[9] = {0};
        int i, j;

        for (i = row; i < row + 3; i++) {
            for (j = col; j < col + 3; j++) {
                int num = sudoku[i][j];
                if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
                    pthread_exit(NULL);
                } 
                else {
                    validityArray[num - 1] = 1;
                }
            }
        }

        valid[row + col/3] = 1;
        pthread_exit(NULL);
    }

    pthread_t threads[num_threads];
    int threadIndex = 0;
    int i,j;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (i%3 == 0 && j%3 == 0) {
                parameters *data = (parameters *) malloc(sizeof(parameters));
                data->row = i;
                data->column = j;
                pthread_create(&threads[threadIndex++], NULL, is3x3Valid, data);
            }

            if (i == 0) {
                parameters *columnData = (parameters *) malloc(sizeof(parameters));
                columnData->row = i;
                columnData->column = j;
                pthread_create(&threads[threadIndex++], NULL, isColumnValid, columnData);
            }

            if (j == 0) {
                parameters *rowData = (parameters *) malloc(sizeof(parameters));
                rowData->row = i;
                rowData->column = j;
                pthread_create(&threads[threadIndex++], NULL, isRowValid, rowData);
            }
        }

    }

    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    for (i = 0; i < num_threads; i++) {
        if (valid[i] == 0) {
            printf("Sudoku solution is invalid!\n");
            return 0;
        }
    }   

    printf("Sudoku solution is valid!\n");
    return 1;
}


