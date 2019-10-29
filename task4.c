#include <sys/time.h> 
#include <stdio.h>
 #include <pthread.h> 
#include <errno.h> 
#include <time.h> 
#include <stdlib.h>
//S ОСТАНОВИТЬ , Q ПРОДОЛЖИТЬ
#define SIZE 8
#define t 4
time_t timer ;
int N;
pthread_mutex_t region_mutex = PTHREAD_MUTEX_INITIALIZER; //объявление и инициализация мьютекса
pthread_cond_t space_available = PTHREAD_COND_INITIALIZER; //условная переменная 
pthread_cond_t data_available = PTHREAD_COND_INITIALIZER; //вторая условная переменная
int b[SIZE];//буфер
int TIME = 0; //таймер
void main() 
{ 
    srand(time(NULL));
    
    printf("Введите число N = ");
    scanf(" %d", &N);//введите N;
    pthread_t producer_thread; //объявление потока заполняющего буффер
    pthread_t consumer_thread; //объявление потока читающего буффер
    void *producer();    //При создании потока с использованием pthreads вам нужно указать его на  функцию,чтобы он начал выполнение. Функция должна возвращать void * и принимать один аргумент void * . 
    void *consumer(); 
    pthread_create(&consumer_thread, NULL, consumer, NULL);   //Этот метод принимает четыре аргумента: указатель на переменную pthread_t , любые дополнительные атрибуты (не беспокойтесь об этом на данный момент - просто установите его в NULL), указатель на вызываемую функцию (то есть: имя точки входа) и указатель передается как аргумент функции.
    pthread_create(&producer_thread, NULL, producer, NULL); 
    pthread_join(consumer_thread,NULL); //Когда только что созданный поток завершил выполнение бит, нам нужно объединить все. 
    
} 

void add_buffer() //функция добавления в буфер
{ 
 
    if (b[6]<=0) 
    {
        b[0]++;//добавили ко всем А
        b[2]++;//добавили в очередь А
        printf("Пришел сотрудник Apple №%i \n",  b[0]);
    }
    
    else if(b[6]>0) 
        { 
            b[1]++;//добавили М
            b[3]++;//добавили очередь М
            printf("Пришел сотрудник Microsoft №%i \n ", b[1]);
        }

   }
void add_ferry(void) 
{ 
   if (b[4]+b[5]==2*N && b[7] == 0)
    {
        b[4]=0;
        b[5]=0;
        b[7]=1;
        printf("Паром ушел \n");
    }

    if (b[7]==0)//если паром на месте
    {
        if (b[2]>b[3])
        {
            if ((b[5]<=N && b[4]<N) || (b[5]==0 && b[4]<2*N))
            {
                if (b[2]>=2*N)
                {
                    b[4] +=2*N;
                    b[2]-=2*N;
                }
                else
                {
                    b[4] +=b[2];
                    b[2]-=b[2];
                }
            }       
          // else
                if ((b[4]<=N && b[5]<N) || (b[4]==0 && b[5]<2*N))
                {
                    if (b[3]>=2*N)
                    {
                        b[5] +=2*N;
                        b[3]-=2*N;
                    }
                    else
                    { 
                        b[5] +=b[3];
                        b[3]-=b[3];
                    }
                }  
        }
        else
        {
            if ((b[4]<=N && b[5]<N) || (b[4]==0 && b[5]<2*N))
                {
                    if (b[3]>=2*N)
                    {
                        b[5] +=2*N;
                        b[3]-=2*N;
                    }
                    else
                    { 
                        b[5] +=b[3];
                        b[3]-=b[3];
                    }
                }  
         
            if ((b[5]<=N && b[4]<N) || (b[5]==0 && b[4]<2*N))
            {
                if (b[2]>=2*N)
                {
                    b[4] +=2*N;
                    b[2]-=2*N;
                }
                else
                {
                    b[4] +=b[2];
                    b[2]-=b[2];
                }
            }       
            
    } 
}
    if (b[7]==1) 
        { 
           
            TIME++;
            printf("\n");  
        }
        else 
            { 
                TIME = 0;
                printf("На пароме : %i сотрудников Apple, %i сотрудников Microsoft \n",b[4],b[5]);
                printf("\n");
            }
     printf(" Очередь: %i сотрудников Apple, %i сотрудников Microsoft\n", b[2],b[3]);
     if(TIME == t)
        {
            b[7] = 0;
            printf("Паром готов к загрузке.\n");
        }
}

 void *producer() 
{
    for (;;) 
    {sleep(2);
        b[6] = -5+rand()%10 ;
        pthread_mutex_lock(&region_mutex);//Успешный вызов блокировки мьютекса посредством mutex_lock () вызовет другой поток, который также пытается заблокировать один и тот же мьютекс, чтобы блокировать до тех пор, пока поток владельца не разблокирует его с помощью mutex_unlock () . Темы в рамках одного процесса или внутри других процессов могут совместно использовать мьютексы.
        if (5<0) 
        pthread_cond_wait(&data_available, &region_mutex);
        timer = time(NULL);
        printf("%s ", ctime(&timer));
        add_buffer(); 
        //pthread_cond_signal(&data_available); //Вызов pthread_cond_signal () блокирует по крайней мере один из потоков, которые блокируются при указанной переменной условия cond (если какие-либо потоки заблокированы на cond ).
pthread_cond_signal(&space_available);         
pthread_mutex_unlock(&region_mutex);
    }
    pthread_exit(NULL); 
} 

void *consumer()
{ 
   
    for (;;) //типо пока нет ошибки
    {  sleep(2);//printf("поток работает \n");
        pthread_mutex_lock(&region_mutex); 

       //while ((b[2]==0 && b[3])==0) 
        while (5<0)
        pthread_cond_wait(&data_available, &region_mutex);
       timer = time(NULL);
        printf("%s ", ctime(&timer));
        add_ferry();
        pthread_cond_signal(&space_available); 
        pthread_mutex_unlock(&region_mutex);
    } 
    pthread_exit(NULL); 
} 
