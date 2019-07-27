# Process-Synchronisation
# Using Semaphores for synchronising the processes

# compile using : gcc coffee.c -o  coffee -lrt -pthread
 run by : ./coffee

# program flow : 

1. As this program is based on 4 processes(one Agent and three Coffee lovers ) we have to synchronise them so as only one process can do a task at a time .So to implement this program we have used 4 semaphore variables to synchronise the task which are created in shared memory using shm_open() function in C and mapped to each processes memory. 

2. First semaphore(pa) is used for Agent process (in our case it is the parent process) who generate two items randomly  and then other three semaphores are used by other three processes(coffee lovers i.e child process), Agent process increment the value for the semaphore of third child and subsequently third child checks wethere it has the third item or not if it has then use it and increment the parent process's semaphore otherwise it increment the value of second child's semaphore and then second did same for first child's semaphore . 
