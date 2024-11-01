// Ian Scheetz
// 11/1/24

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

//The consumer takes items from the table
//The table can hold two items
//When the table is empty, the consumer waits

struct sharedBuffer
{
    sem_t mutex;
    sem_t empty;
    int table[2];
};

#define sharedAddress 0x1234

void * ConsumerThread()
{
    printf("Created consumer thread. \n");

    int shm;
    const int size = 4096;
    struct sharedBuffer *ptr;

    //attach to shared buffer
    shm = shmget(sharedAddress, sizeof(struct sharedBuffer), 0644|IPC_CREAT);
    ptr = shmat(shm, NULL, 0);

    sleep(1);

    int loops = 3; // number of times consumer will take from table
    for(int l = 0; l < loops; l++)
    {
        sleep(1);
        sem_wait(&ptr->mutex);

        printf("Consumer using table. \n");
        
        int items = 1;
        int tableIndx = 1;
        sem_getvalue(&ptr->empty, &items);

        while(items < 2)
        {
            int x = ptr->table[tableIndx];		
			sem_post(&ptr->empty);
			printf("Item %d consumed: ", tableIndx);
			printf("%d\n", x);
			++tableIndx;
			sem_getvalue(&ptr->empty, &items);	
        }

        printf("Consumer has taken everything from table. \n");
        sem_post(&ptr->mutex);
    }

    shmdt(ptr);	
	shmctl(shm, IPC_RMID, NULL);
}

int main () 
{
    pthread_t consumer;
	pthread_create (&consumer, NULL, ConsumerThread, NULL);
	pthread_join(consumer, NULL);					

	return 0;

}