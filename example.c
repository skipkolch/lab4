#include <sys/time.h> 
#include <stdio.h>
 #include <pthread.h> 
#include <errno.h> 
#include <time.h> 
#include <stdlib.h>
//переделать отправку парома, после отправки паром пропускает одно событие, затем возвращается, а здесь сравниваются очереди и кто пойдет на паром// рандомный 0 не работает//заменить а на сигнал
#define SIZE 9
time_t timer ;
int N,a=1;
pthread_mutex_t region_mutex = PTHREAD_MUTEX_INITIALIZER; //мютекс - аналог одноместного семафора, служащий в программировании для синхронизации одновременно выполняющихся потоков.Взаимные блокировки исключения (мьютексы) предотвращают одновременное выполнение несколькими потоками критических разделов кода, которые обращаются к общим данным (т. Е. Мьютексы используются для сериализации выполнения потоков);
pthread_cond_t space_available = PTHREAD_COND_INITIALIZER; //условная переменная В случаях, когда атрибуты переменной по умолчанию являются подходящими, макрос PTHREAD_COND_INITIALIZER может использоваться для инициализации переменных состояния, которые статически распределены. Эффект должен быть эквивалентен динамической инициализации вызовом pthread_cond_init () с параметром attr, указанным как NULL, за исключением того, что не выполняются проверки ошибок.
pthread_cond_t data_available = PTHREAD_COND_INITIALIZER; 
int b[SIZE];
int TIME = 0; //int size = 0; //int front, rear = 0; 
void main() 
{ 
srand(time(NULL));
printf("Введите число N = ");
scanf(" %d", &N);//введите N;
pthread_t producer_thread; 
pthread_t consumer_thread; 
void *producer(); //При создании потока с использованием pthreads вам нужно указать его на функцию, чтобы он начал выполнение. Функция должна возвращать void * и принимать один аргумент void * . 
void *consumer(); 
pthread_create(&consumer_thread, NULL, consumer, NULL); //Этот метод принимает четыре аргумента: указатель на переменную pthread_t , любые дополнительные атрибуты (не беспокойтесь об этом на данный момент - просто установите его в NULL), указатель на вызываемую функцию (то есть: имя точки входа) и указатель передается как аргумент функции.
pthread_create(&producer_thread, NULL, producer, NULL); 
pthread_join(consumer_thread,NULL); //Когда только что созданный поток завершил выполнение бит, нам нужно объединить все. Это выполняется с помощью функции pthread_join которая принимает два параметра: переменную pthread_t используемую при вызове pthread_create (а не указатель на этот раз), и указатель на указатель возвращаемого значения (не беспокойтесь об этом сейчас - просто установите его в NULL ).
} 

// заполняем паром из очереди а если очередь пуста то заполняем из х и у 

void add_buffer(int i) 
{ //lдобавить условие если в очереди есть люди то грузить их
	a=-1;
	b[6]=i;
	b[8] = 4;
	if (b[4]+b[5]==2*N && b[7] == 0)
	{
		b[4]=0;
		b[5]=0;
		b[7]=1;
	}
	else if (b[4]+b[5]<2*N) b[7]=0;
		if (i <= 0)//когда 0 не работает
		{
				b[0]++;
				b[2]++;
			if ((b[4]<=b[5] && b[5]<=N && b[4]<N) || (b[5]==0 && b[4]<2*N))
			{
			b[4] ++;
			b[2]--;
			}
		}
		else if (i > 0)
			{
				b[1]++;
				b[3]++;
			if ((b[4]>=b[5] && b[4]<=N && b[5]<N) || (b[4]==0 && b[5]<2*N))
				{
					b[5] ++;
					b[3]--;
				}
			}

} 

int get_buffer(int i) 
{ 
	a=1;
	int v;
	v = b[i]; 

	return v; 
}

 void *producer() 
{
	int rand1;
	int COUNT_TIME;
	while (1) 
	{
  
	rand1 = -5+rand()%10 ;

//while(!rand1) // если равно 0;
//rand1 = rand()%361 + 1;

	pthread_mutex_lock(&region_mutex);//Успешный вызов блокировки мьютекса посредством mutex_lock () вызовет другой поток, который также пытается заблокировать один и тот же мьютекс, чтобы блокировать до тех пор, пока поток владельца не разблокирует его с помощью mutex_unlock () . Темы в рамках одного процесса или внутри других процессов могут совместно использовать мьютексы.
	if (a == -1)
 	pthread_cond_wait(&space_available, &region_mutex); //Функция pthread_cond_wait() используется, чтобы атомарно освободить мьютекс и заставить вызывающий поток блокироваться по переменной состояния. Функция pthread_cond_wait() возвращает 0 - после успешного завершения. Любое другое значение указывает, что произошла ошибка

	sleep(2);
//for(int i=0; time(NULL)<20000;i++)

	add_buffer(rand1); 
	pthread_cond_signal(&data_available); //Вызов pthread_cond_signal () блокирует по крайней мере один из потоков, которые блокируются при указанной переменной условия cond (если какие-либо потоки заблокированы на cond ).
	pthread_mutex_unlock(&region_mutex);
	}
 	pthread_exit(NULL); 
} 

void *consumer()
 { int all_Apple,all_Microsoft,count_A,count_M,Q1,Q2;int rand1;int i=0;int count;
	while (1)//типо пока нет ошибки
	{
		pthread_mutex_lock(&region_mutex); 
		if (a == 1) 
			pthread_cond_wait(&data_available, &region_mutex);
		timer = time(NULL);//возвращает время в секундах с 1 января 1970
	all_Apple = get_buffer(0);
	all_Microsoft = get_buffer(1);
	Q1= get_buffer(2);
	Q2= get_buffer(3);
	count_A= get_buffer(4);
	count_M= get_buffer(5);
	rand1=get_buffer(6);
	count = get_buffer(7);


//printf("рандом №%i ",rand1);
//printf("Пришел сотрудник Apple №%i ",b[0]);
		printf("%s ", ctime(&timer));

	if (count==1) 
	{ 
   	 	printf("Паром отправляется ");
    	TIME = 0;

    	if (rand1 <= 0) 
            printf("Пришел сотрудник Apple №%i ", rand1, all_Apple);
        	else if (rand1 > 0) 
        	printf("Пришел сотрудник Microsoft №%i ",rand1, all_Microsoft);
        	printf(" Очередь: %i сотрудников Apple, %i сотрудников Microsoft\n", Q1,Q2);

        	printf("\n");
   
	}
	else if (count == 0)
	{ 
    	if (rand1 <= 0) 
        printf("Пришел сотрудник Apple №%i ", rand1, all_Apple);
    	else if (rand1 > 0) 
    	printf("Пришел сотрудник Microsoft №%i ",rand1, all_Microsoft);

    	printf("На пароме : %i сотрудников Apple, %i сотрудников Microsoft \n",count_A,count_M);
    	printf(" Очередь: %i сотрудников Apple, %i сотрудников Microsoft\n", Q1,Q2);

    	printf("\n");
	}

	 pthread_cond_signal(&space_available); 
	pthread_mutex_unlock(&region_mutex);
 	//i++;
	TIME++;

	if(TIME == b[8])
	{
   	 TIME = 0;
  	  count = 0;
   	 printf("Паром готов к загрузке.");
	}
	//sleep(2);
 } 



	pthread_exit(NULL); 
} 
