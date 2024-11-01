# OperatingSystems-Assignment-1
Ian Scheetz
11/1/24

Included are two files, producer.c and consumer.c. To run the program, put both files in the same folder and run:

$ gcc producer.c -pthread -lrt -o producer

$ gcc consumer.c -pthread -lrt -o consumer

$ ./producer & ./consumer &

Each process will create a thread that will then connect to a shared buffer. The producer and consumer will loop 3 times, so the table will be filled, then the consumer will take all items of the table, then repeat twice. The producer will create a random integer from 1-10 for each spot on the table, then the consumer will take these integers off the table and display them.

# Example output:
![Screenshot_2024-11-01_18-20-33](https://github.com/user-attachments/assets/4d90f61c-2938-48c0-ab74-a3b59a7dd66a)
