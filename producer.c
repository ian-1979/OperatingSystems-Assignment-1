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

struct sharedBuffer
{
    sem_t mutex;
    sem_t empty;
    int table[2];
};

#define sharedAddress 0x1234

//The producer puts items on the table
//The table can hold two items
//When the table is full, the producer waits

void * ProducerThread()
{
    printf("Created producer thread. \n");

    int shm;
    const int size = 4096;
    struct sharedBuffer *ptr;

    //create shared buffer
    shm = shmget(sharedAddress, sizeof(struct sharedBuffer), 0644|IPC_CREAT);
    ptr = shmat(shm, NULL, 0);

    //initialize sems
    sem_init(&ptr->mutex, 1, 1);
	sem_init(&ptr->empty, 1, 2);	

    int loops = 3; // number of times producer will fill table
    for(int l = 0; l < loops; l++)
    {
        sleep(1);
        sem_wait(&ptr->mutex);

        printf("Producer using table. \n");
        
        int items = 0;
        int tableIndx = 1;
        sem_getvalue(&ptr->empty, &items);

        while(items > 0)
        {
            int x = rand()%10;
			ptr->table[tableIndx] = x;
			sem_wait(&ptr->empty);	
			printf("Item %d produced: ", tableIndx);
			printf("%d\n", x);
			++tableIndx;
			sem_getvalue(&ptr->empty, &items);	
        }

        printf("Producer has filled table. \n");
        sem_post(&ptr->mutex);
    }
    
    sem_destroy(&ptr->mutex);
	sem_destroy(&ptr->empty);
	shmdt(ptr);
}

int main() 
{

	pthread_t producer;
	pthread_create (&producer, NULL, ProducerThread, NULL);	
	pthread_join(producer, NULL);
	
	return 0;
}
