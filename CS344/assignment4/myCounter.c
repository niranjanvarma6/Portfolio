#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
//declared global variables
int myCount = 0;
pthread_cond_t myCond1;
pthread_cond_t myCond2;
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
// consumer and producer functions using module 2.3 pg 50 and 51. Both functions are pointers to accomodate the pthread_create functions
//function holding the consumer loop
void* consumer(){
    //CREATED MESSAGE
    printf("CONSUMER THREAD CREATED \n");
    fflush(stdout); 
    while(1){
        //lock the mutex before potential change of myCount
        printf("CONSUMER: myMutex locked\n");
        fflush(stdout);
        pthread_mutex_lock(&myMutex);
        while((myCount % 2) == 1){ 
            //consumer held only when myCount is odd
            printf("CONSUMER: waiting on myCond2 \n");
            fflush(stdout);
            pthread_cond_wait(&myCond2, &myMutex);
        }
        //myCount status update
        printf("myCount: %d -> %d \n", myCount, myCount + 1);   
        fflush(stdout);     
        myCount++;
        printf("CONSUMER: myMutex unlocked \n");
        fflush(stdout);
        //unlock the mutex only when myCount(after incrementing) is odd
        pthread_mutex_unlock(&myMutex);
        printf("CONSUMER: signaling myCond1 \n"); 
        fflush(stdout);
        // signal the producer that there is space in the buffer
        pthread_cond_signal(&myCond1);
    }
}
//function holding the producer loop
void* producer(){
    while(1){
        //lock the mutex before potential change of myCount
        pthread_mutex_lock(&myMutex);
        printf("PRODUCER: myMutex locked\n");
        fflush(stdout);
        while((myCount % 2) == 0){ 
            // producer held when myCount is even 
            printf("PRODUCER: waiting on myCond1 \n");
            fflush(stdout);
            pthread_cond_wait(&myCond1, &myMutex);
        }
        //myCount status update
        printf("myCount: %d -> %d \n", myCount, myCount + 1);
        fflush(stdout);
        myCount++;
        //if statement used to terminate the program after final myCOunt update
        if(myCount == 10){
            break;
        }
        printf("PRODUCER: myMutex unlocked \n");
        fflush(stdout);
        //unlock the mutex only when myCount(after incrementing) is even
        pthread_mutex_unlock(&myMutex);
        printf("PRODUCER: signaling myCond2 \n");
        fflush(stdout);
        //send signal to the consumer that the buffer has space
        pthread_cond_signal(&myCond2);
    }
}
//function to make the thread
void make_thread(){
    printf("PROGRAM START \n");
    fflush(stdout);
    // https://www.geeksforgeeks.org/multithreading-c-2/ was the source behind this the commands and concepts used to execute the program
    pthread_t thread_id1;
    pthread_t thread_id2;
    //creating the actual threads using the functions defined above
    pthread_create(&thread_id1, NULL, consumer, NULL); // called first for the thread creation message to appear correctly 
    pthread_create(&thread_id2, NULL, producer, NULL); 
    pthread_join(thread_id2, NULL); // producer thread is waited on until it terminates for the program end statement 
    printf("PROGRAM END \n");
    fflush(stdout);
}
//making the main as simple as possible
int main(){
    make_thread(); //call to make the thread for the program
    return 0;
}

