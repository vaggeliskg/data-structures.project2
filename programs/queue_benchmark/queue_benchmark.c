#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADTQueue.h"
#include "ADTList.h"
#include "ADTStack.h"


int* create_int(int value) {
    int* p = malloc(sizeof(int));
    *p = value;
    return p;
}
//main με ορίσματα από τη γραμμή εντολών για real | amortized
int main(int argc, char **argv) {
    FILE *filepntr = fopen("CSV_FORMAT.csv", "w"); // opens the .csv file to write in it
    Queue queue = queue_create(NULL);
    //case1 = insert , case2 = insert , remove
    int case1 = 1;
    int case2 = 0;
    int remove = 0;
    int sum_real_time = 0;
    int sum_amortized_time = 0;
    int amortized_steps  = 0;
    //calculates the amortized  time steps
    if(strcmp(argv[1], "amortized") == 0) {
        for(int i = 0 ; i < 9000 ; i++) {
            if(case1 == 1) {
                Pointer value = create_int(i);
                queue_insert_back(queue,value);
                sum_amortized_time += queue_steps(queue);
                case1 = 0;
                case2 = 1;
                amortized_steps = sum_amortized_time / (i+1);
                fprintf(filepntr, "%d,%d\n", i+1, amortized_steps);
                continue;
            }          
            if(case2 == 1) {
                Pointer value = create_int(i);
                queue_insert_back(queue,value);
                sum_amortized_time +=queue_steps(queue);
                case1 = 0;
                case2 = 0;
                remove = 1;
                amortized_steps = sum_amortized_time / (i+1);
                fprintf(filepntr, "%d,%d\n", i+1, amortized_steps);
                continue;
            }
            if(remove == 1) {
                queue_remove_front(queue);
                sum_amortized_time += queue_steps(queue);
                remove = 0;
                case1 = 1;
                case2 = 0;
                amortized_steps = sum_amortized_time / (i+1);
                fprintf(filepntr, "%d,%d\n", i+1, amortized_steps);
            }
        }
    }
    //calulates real time steps
    else if(strcmp(argv[1], "real") == 0) {
        for(int i = 0 ; i < 9000 ; i++) {
            if(case1 == 1) {
                Pointer value = create_int(i);
                queue_insert_back(queue,value);
                case1 = 0;
                case2 = 1;
                continue;
            }
            if(case2 == 1) {
                Pointer value = create_int(i);
                queue_insert_back(queue,value);
                case1 = 0;
                case2 = 0;
                remove = 1;
               continue;
            }
            if(remove == 1) {
                int size = queue_size(queue);
                queue_remove_front(queue);
                sum_real_time = queue_steps(queue);
                case1 = 1;
                case2 = 0;
                remove = 0;
                int real_steps = sum_real_time;
                fprintf(filepntr, "%d,%d\n",size, real_steps);
            }
        } 
    }
}