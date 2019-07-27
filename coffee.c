#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include<sys/wait.h>
#include<time.h> 
 
typedef struct {
    sem_t pa;
    sem_t ch1;
    sem_t ch2;
    sem_t ch3;
} SemPair;
 
int main(int argc, const char *argv[]) {
    int shm = shm_open("/test", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    int shm2 = shm_open("/test2", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shm, sizeof(sem_t));
    ftruncate(shm2, sizeof(int));
    SemPair *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
    int *count = mmap(NULL, 4*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm2, 0);
 
    sem_init(&(sem->pa), 1, 1);
    sem_init(&(sem->ch1), 1, 0);
    sem_init(&(sem->ch2), 1, 0);
    sem_init(&(sem->ch3), 1, 0);

    int i=0 ;int stat_loc ;
    printf("Enter the number of iteration for playing the Game : ") ;
    scanf("%d" , &count[3]) ;

    srand(time(0)) ;

    pid_t ch1  , ch2 , ch3 ;
    
    if ((ch1  = fork()) < 0) // first child creation
        printf("child1 is not created\n") ;

    else if(ch1 > 0){
        if ((ch2  = fork()) < 0)   // Second child creation
            printf("child1 is not created\n") ;

        else if(ch2 > 0){
            if ((ch3  = fork()) < 0)    // third child creation
                printf("child1 is not created\n") ;

            else if(ch3 > 0){
                
                while(count[3]){
                    sleep(1) ;
                    
                    sem_wait(&(sem->pa)) ; // down semaphore to restrict parent , not run again before any child execution 
                    count[0] = 0 ;
                    count[1] = 0 ;
                    count[2] = 0 ;
                    printf("\nAgent process " ) ;
                    
                    int a = rand()%3 ; 
                    int b = (a+1)%3 ; 
                    count[a] = 1 ;
                    count[b] = 1 ;

                    if(a == 0 && b == 1)
                        printf("generated items are :  sugar and cofee powder \n ") ;

                    else if(a == 1 && b == 2)
                        printf("generated items are :  cofee powder and milk \n ") ;

                    else if(a == 2 && b == 0)
                        printf("generated items are :  milk and sugar \n ") ;

                    sem_post(&(sem->ch3)) ; // up operation for waking a third child
                    
                    count[3]-- ;
                    
                }
                wait(NULL);
        
            }
            else{
                //child3
                while(count[3]){
                   sleep(1) ;
                    sem_wait(&(sem->ch3));
                    if(!count[2]){
                        
                        printf("coffee lover 3 has made coffee as it has remaining item i.e. milk \n") ;
                        sem_post(&(sem->pa)) ; // third child wake up the parent after using the resources
                    }
                    else 
                        sem_post(&(sem->ch2)) ; // third child will wakeup the second child if it does not have the required item 
                }
            }
        }
        else{
                //child2
            while(count[3]){
                
                sleep(1) ;
                sem_wait(&(sem->ch2));
                
                if(!count[1]){
                    
                    printf("coffee lover 2 has made coffee as it has remaining item i.e. coffee powder\n") ;
                    sem_post(&(sem->pa)) ;
                }
                else
                    sem_post(&(sem->ch1)) ;
            }
        }
    }
    else{
            //child1
        while(count[3]){
        
            sem_wait(&(sem->ch1));
            
            if(!count[0]){
                printf("coffee lover 1 has made coffee as it has remaining item i.e. sugar \n") ;
                sem_post(&(sem->pa)) ;
            }

        }
    }
   

    sem_destroy(&(sem->pa)) ;
    sem_destroy(&(sem->ch1)) ;

    sem_destroy(&(sem->ch2)) ;
    sem_destroy(&(sem->ch3)) ; 
    
    return 0;
}