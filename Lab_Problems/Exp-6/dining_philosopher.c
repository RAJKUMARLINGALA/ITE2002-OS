#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t mutex_fork[5]; // 5 mutexes for each fork
pthread_key_t phil_num;

void *philosopher_func(void *arg)
{
printf("%d\n",*((int*)arg));
pthread_setspecific(phil_num,arg);

printf("Philosopher %d is thinking.\n",*(int*)pthread_getspecific(phil_num));
sleep(3);

/*philosopher number phi_no will now pick up two forks.*/
pthread_mutex_lock(&mutex_fork[*(int*)pthread_getspecific(phil_num)]);
pthread_mutex_lock(&mutex_fork[(*(int*)pthread_getspecific(phil_num)+1)%5]);

printf("Philosopher %d is eating.\n",*(int*)pthread_getspecific(phil_num));
sleep(2);

pthread_mutex_unlock(&mutex_fork[*(int*)pthread_getspecific(phil_num)]);
pthread_mutex_unlock(&mutex_fork[(*(int*)pthread_getspecific(phil_num)+1)%5]);

printf("Philosopher %d finished eating.\n",*(int*)pthread_getspecific(phil_num));

return NULL;
}

int main(int argc,char **argv)
{
pthread_t thread_philosopher[5];
pthread_key_create(&phil_num,NULL);

int i;
for(i=0;i<5;i++)
pthread_mutex_init(&mutex_fork[i],NULL);

for(i=0;i<5;i++)
{
//printf("%d\n",i);
pthread_create(&thread_philosopher[i],NULL,philosopher_func,(void*)&i);
}
for(i=0;i<5;i++)
pthread_mutex_destroy(&mutex_fork[i]);// freeing the space occupied by mutex

for(i=0;i<5;i++)
pthread_join(thread_philosopher[i],NULL);

return 0;
}
