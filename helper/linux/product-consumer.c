#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define MAXLENGTH 10    //the maxlength of queue

typedef char * datatype;
typedef struct node {    //define node
	datatype name;
	struct node *next;
} node;
typedef struct queue {    //define queue
	node *front, *rear;
	int len;
} queue;

void queue_init(queue *q)
{
	q->front = q->rear = NULL;
	q->len = 0;
}

void  queue_put(queue *q, datatype new_name)//入队
{
	node *mynode = (node *)malloc(sizeof(node));
	mynode->name = new_name;
	mynode->next = NULL;
	if (q->rear)
		q->rear->next = mynode;
	q->rear = mynode;
	if (q->front == NULL)
		q->front = mynode;
	q->len++;
}

datatype queue_get(queue *q) //出队
{
	node *mynode;
	datatype myname;
	if (q->front != NULL)
		mynode = q->front;
	myname = mynode->name;
	q->front = q->front->next;
	q->len--;
	free(mynode);
	return myname;
}

void queue_print(queue *q) //print queue
{
	node *tmp = q->front;
	while(tmp != NULL)
	{
		printf("%s ", tmp->name);
		tmp = tmp->next;
	}
	printf("\n");
}

/*生产者与消费者函数*/
/*define mutex and condtion var*/
pthread_cond_t q_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t q_not_empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

/*
 * producer function
 */
void producer(void *q)
{
	//    printf("start porducer:\n");
	queue *qt = q;    //传入的队列
	while(1)
	{
		pthread_mutex_lock(&qlock);
		//        printf("producer has locked the qlock\n");
		while(qt->len >= MAXLENGTH)    //queue is full
		{
			//            printf("producer is going to waiting\n");
			pthread_cond_wait(&q_not_full, &qlock);
		}
		queue_put(qt, "* ");
		//        printf("after producer: queue's length is %d\n", qt->len);
		pthread_mutex_unlock(&qlock);
		pthread_cond_signal(&q_not_empty);

		//        printf("producer has unlocked the qlock\n");
		//        sleep(1);
	}
}

/*
 * consumer function
 */
void consumer(void *q)
{
	//    printf("start consumer:\n");
	queue *qt = q;
	while(1)
	{
		pthread_mutex_lock(&qlock);
		//        printf("consumer has locked the qlock\n");
		while(qt->len <= 0)    //queue is empty
		{
			//            printf("consumer is going to waiting\n");
			pthread_cond_wait(&q_not_empty, &qlock);
		}
		datatype back_name = queue_get(qt);
		//        printf("after consumer, queue's length is %d\n", qt->len);
		pthread_mutex_unlock(&qlock);
		pthread_cond_signal(&q_not_full);
		//        now process the back_name
		//        printf("cousumer has unlocked the qlock\n");
		//        sleep(1);
	}
}

//gcc编译时加上-lpthread
int main() {
	pthread_t tid1, tid2;
	queue *q=(queue *)malloc(sizeof(queue));
	queue_init(q);

#ifdef DBG_TEST
	queue_put(q, "one");
	queue_put(q, "two");
	queue_put(q, "three");
	queue_get(q);
	printf("len = %d\n", q->len);
	queue_print(q);
#endif
	long stime = clock();
	long etime = clock();

	pthread_create(&tid1, NULL, (void *)producer, (void *)q);
	pthread_create(&tid2, NULL, (void *)consumer, (void *)q);

	while((float)(etime-stime)/CLOCKS_PER_SEC < 0.00001)
	{
		etime = clock();
	}
	return 0;
}
