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

int main(int argc, char **argv) {
    FILE *filepntr = fopen("CSV_FORMAT.csv", "w");
    Queue queue = queue_create(NULL);
    
    int insert1 = 1;
    int insert2 = 0;
    int remove = 0;
    int sum_real_time = 0;
    int sum_amortized_time = 0;
    if(strcmp(argv[1], "amortized") == 0) {
        for(int i = 0 ; i < 9000 ; i++) {
            if(insert1 == 1) {
                Pointer value = create_int(i);
                queue_insert_back(queue,value);
                sum_amortized_time += queue_steps(queue);
                insert1 = 0;
                insert2 = 1;
                int amortized_steps = sum_amortized_time / (i+1);
                    fprintf(filepntr, "%d,%d\n", i+1, amortized_steps);
            }
            continue;
            if(insert2 == 1) {
                Pointer value = create_int(i);
                queue_insert_back(queue,value);
                sum_amortized_time +=queue_steps(queue);
                insert1 = 1;
                insert2 = 0;
                remove = 1;
                int amortized_steps = sum_amortized_time / (i+1);
                fprintf(filepntr, "%d,%d\n", i+1, amortized_steps);
            }
            if(remove == 1) {
                queue_remove_front(queue);
                sum_amortized_time += queue_steps(queue);
                remove = 0;
                insert1 = 1;
                insert2 = 0;
                int amortized_steps = sum_amortized_time / (i+1);
                 fprintf(filepntr, "%d,%d\n", i+1, amortized_steps);
            }
        }
    }
    else if(strcmp(argv[1], "real") == 0) {
        for(int i = 0 ; i < 9000 ; i++) {
            if(insert1 == 1) {
                Pointer value = create_int(i);
                queue_insert_back(queue,value);
                insert1 = 0;
                insert2 = 1;
            }
            continue;
            if(insert2 == 1) {
                Pointer value = create_int(i);
                queue_insert_back(queue,value);
                insert1 = 1;
                insert2 = 0;
                remove = 1;
            }
            if(remove == 1) {
                queue_remove_front(queue);
                sum_real_time += queue_steps(queue);
                remove = 0;
                int real_steps = sum_real_time;
                 fprintf(filepntr, "%d,%d\n", i+1, real_steps);
            }
        } 
    }
}