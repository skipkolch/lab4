#include <sys/time.h> 
#include <stdio.h>
#include <pthread.h> 
#include <errno.h> 
#include <time.h> 
#include <stdlib.h>

//S ОСТАНОВИТЬ , Q ПРОДОЛЖИТЬ
#define SIZE 8
#define t 4 // время за которое переправляется паром



pthread_mutex_t region_mutex = PTHREAD_MUTEX_INITIALIZER; //объявление и инициализация мьютекса
pthread_cond_t space_available = PTHREAD_COND_INITIALIZER; //условная переменная 
pthread_cond_t data_available = PTHREAD_COND_INITIALIZER;

time_t timer; // год, месяц, дата
  
 //вторая условная переменная time_t timer ;

int buff[SIZE];//буфер

/*
 0 - все apple
 1 - все майкрософты
 2 - очередь apple
 3 - очередь майкрософт
 4 - apple на пароме
 5 - майкрософт на пароме 
 6 - рандомное число 
 7 - управление паромом (пришел-ушел)
*/


int N,a=1;
int TIME = 0; //таймер

void main() 
{ 
    srand(time(NULL));
    printf("Введите число N = ");
    scanf(" %d", &N);//введите N;

    pthread_t producer_thread; //объявление потока заполняющего буффер
    pthread_t consumer_thread; //объявление потока читающего буффер

    void *producer(); //При создании потока с использованием pthreads вам нужно указать его на  функцию,чтобы он начал выполнение. Функция должна возвращать void * и принимать один аргумент void * . 
    void *consumer(); 


    pthread_create(&consumer_thread, NULL, consumer, NULL); //Этот метод принимает четыре аргумента: указатель на переменную pthread_t , любые дополнительные атрибуты (не беспокойтесь об этом на данный момент - просто установите его в NULL), указатель на вызываемую функцию (то есть: имя точки входа) и указатель передается как аргумент функции.
    pthread_create(&producer_thread, NULL, producer, NULL); 

    pthread_join(consumer_thread,NULL); //Когда только что созданный поток завершил выполнение бит, нам нужно объединить все. 
} 

// заполняем паром из очереди а если очередь пуста то заполняем из х и у 

void add_buffer(int i) //функция добавления в буфер
{ 
    a = -1;//УСЛОВИЕ синхронности

    buff[6] = i;//Рандомная переменная 

    if (i <= 0) 
    {
        buff[0]++;//добавили ко всем А
        buff[2]++;//добавили в очередь А
    }
    
    else if(i>0) 
        {
            buff[1]++;//добавили М
            buff[3]++;//добавили очередь М
        }

    if (buff[4] + buff[5] == 2*N && buff[7] == 0) // Если паром готов уйти
    {
        buff[4] = 0; // обнуляем счетчики
        buff[5] = 0;
        buff[7] = 1;
        printf("Паром ушел \n");
    }

    if (buff[7] == 0)//если паром на месте
    {
        if (buff[2] > buff[3]) // Если очередь эпл больше очереди майков
		{
				// Загрузка майкрософт
		        if ( (buff[5] <= N && buff[4] < N) || (buff[5] == 0 && buff[4] < 2*N) ) 
		        {
		            if (buff[2] >= 2*N) 
		            {
		                buff[4] += 2*N;
		                buff[2] -= 2*N;
		            }
		            else
		            {
		                buff[4] += buff[2];
		                buff[2] -= buff[2];
		            }
		        }  
     
         		// Загрузка Apple
                if ( (buff[4] <= N && buff[5] < N) || (buff[4] == 0 && buff[5] < 2*N) )
                {
                    if (buff[3] >= 2*N)
                    {
                        buff[5] += 2*N;
                        buff[3] -= 2*N;
                    } 
                    else
                    { 
                        buff[5] += buff[3];
                        buff[3] -= buff[3];
                    }
                }  
		}
		else // Если очередь майков больше апл
		{
			if ( (buff[4] <= N && buff[5] < N) || (buff[4] == 0 && buff[5] < 2*N) )
                {
                    if (buff[3] >= 2*N)
                    {
                        buff[5] += 2*N;
                        buff[3] -= 2*N;
                    }
                    else
                    { 
                        buff[5] += buff[3];
                        buff[3] -= buff[3];
                    }
                }  
		}

  
		 if ( (buff[5] <= N && buff[4] < N) || (buff[5] == 0 && buff[4] < 2*N) )
            {
                if (buff[2] >= 2*N)
                {
                    buff[4] += 2*N;
                    buff[2] -= 2*N;
                }
                else
                {
                    buff[4] += buff[2];
                    buff[2] -= buff[2];
                }
            }       
            
    } 
}


int get_buffer(int i) 
{ 
    a = 1;

	int v;
	v = buff[i];

    return v; 
}

 void *producer() 
{
    int rand1;
    int COUNT_TIME;

    while (1) 
    {
        rand1 = -5 + rand()%10 ;
        pthread_mutex_lock(&region_mutex);//Успешный вызов блокировки мьютекса посредством mutex_lock () вызовет другой поток, который также пытается заблокировать один и тот же мьютекс, чтобы блокировать до тех пор, пока поток владельца не разблокирует его с помощью mutex_unlock () . Темы в рамках одного процесса или внутри других процессов могут совместно использовать мьютексы.
        if (a == -1)
        pthread_cond_wait(&space_available, &region_mutex); //Функция pthread_cond_wait() используется, чтобы атомарно освободить мьютекс и заставить вызывающий поток блокироваться по переменной состояния. Функция pthread_cond_wait() возвращает 0 - после успешного завершения. Любое другое значение указывает, что произошла ошибка
        sleep(2);

        add_buffer(rand1); 

        pthread_cond_signal(&data_available); //Вызов pthread_cond_signal () блокирует по крайней мере один из потоков, которые блокируются при указанной переменной условия cond (если какие-либо потоки заблокированы на cond ).
        pthread_mutex_unlock(&region_mutex);
    }
    pthread_exit(NULL); 
} 

void *consumer()
{ 
    int all_Apple,all_Microsoft,count_A,count_M,Q1,Q2,rand1, count, i=0;
    while (1)//типо пока нет ошибки
    {
        pthread_mutex_lock(&region_mutex); // Блокировка мьютекса
        if (a == 1) 
        pthread_cond_wait(&data_available, &region_mutex);

        timer = time(NULL);//возвращает время в секундах с 1 января 1970

        all_Apple = get_buffer(0);
        all_Microsoft = get_buffer(1);
        Q1 = get_buffer(2);
        Q2 = get_buffer(3);
        count_A = get_buffer(4);
        count_M = get_buffer(5);
        rand1 = get_buffer(6);
        count = get_buffer(7);

        printf("%s ", ctime(&timer));

        if (count == 1) 
        { 
            //printf("Паром ушел \n");
            TIME++;
            if (rand1 <= 0) 
                printf("Пришел сотрудник Apple №%i ",  all_Apple);
            else if (rand1 > 0) 
                printf("Пришел сотрудник Microsoft №%i ", all_Microsoft);
            printf(" Очередь: %i сотрудников Apple, %i сотрудников Microsoft\n", Q1,Q2);
            printf("\n");  
        }
        else 
            if (count == 0)
            { 
                TIME = 0;
                if (rand1 <= 0) 
                    printf("Пришел сотрудник Apple №%i ", all_Apple);
                else 
                    if (rand1 > 0) 
                    printf("Пришел сотрудник Microsoft №%i ", all_Microsoft);
                    printf("На пароме : %i сотрудников Apple, %i сотрудников Microsoft \n",count_A,count_M);
                   printf(" Очередь: %i сотрудников Apple, %i сотрудников Microsoft\n", Q1,Q2);
                   printf("\n");
            }
        if(TIME == t)
        {
            buff[7] = 0;
            printf("Паром готов к загрузке.\n");
        }

        pthread_cond_signal(&space_available); 
        pthread_mutex_unlock(&region_mutex); // Разблокировка
    } 
    pthread_exit(NULL); 
} 
