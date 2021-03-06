/* include globals */
#include "unpipc.h"

#define MAXNITEMS 1000000
#define MAXNTHREADS 100

/* globals shared by threads */
int nitems; /* read-only by producer and consumer */
int buff[MAXNITEMS];
struct
{
	pthread_mutex_t mutex;
	int nput; /* next index to store */
	int nval; /* next value to store */
	int nget
} put = {PTHREAD_MUTEX_INITIALIZER};

struct
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int nready; /* number ready for consumer */
} nready = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
/* end globals */

void *produce(void *), *consume(void *);

/* include main */
int main(int argc, char **argv)
{
	int i, nthreads, count[MAXNTHREADS], ccount[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS], tid_consume[MAXNTHREADS];

	if (argc != 3)
		err_quit("usage: prodcons6 <#items> <#threads>");
	nitems = min(atoi(argv[1]), MAXNITEMS);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);

	Set_concurrency(nthreads * 2);
	/* 4create all producers and one consumer */
	for (i = 0; i < nthreads; i++)
	{
		count[i] = 0;
		ccount[i] = 0;
		Pthread_create(&tid_produce[i], NULL, produce, &count[i]);
		Pthread_create(&tid_consume[i], NULL, consume, &count[i]);
	}

	/* wait for all producers and the consumer */
	for (i = 0; i < nthreads; i++)
	{
		Pthread_join(tid_produce[i], NULL);
		printf("count[%d] = %d\n", i, count[i]);
		Pthread_join(tid_consume[i], NULL);
		
		printf("Ccount[%d] = %d\n", i, ccount[i]);
	}

	exit(0);
}
/* end main */

/* include prodcons */
void *
produce(void *arg)
{
	for (;;)
	{
		Pthread_mutex_lock(&put.mutex);
		if (put.nput >= nitems)
		{
			Pthread_mutex_unlock(&put.mutex);
			return (NULL); /* array is full, we're done */
		}
		buff[put.nput] = put.nval;
		put.nput++;
		put.nval++;
		Pthread_mutex_unlock(&put.mutex);

		Pthread_mutex_lock(&nready.mutex);
		if (nready.nready == 0)
			Pthread_cond_signal(&nready.cond);
		nready.nready++;
		Pthread_mutex_unlock(&nready.mutex);

		*((int *)arg) += 1;
	}
}

void *
consume(void *arg)
{
	int i;
	int j;

	for (i = 0; i < nitems; i++)
	{
		Pthread_mutex_lock(&nready.mutex);
		while (nready.nready == 0 && put.nget < nitems)
			Pthread_cond_wait(&nready.cond, &nready.mutex);
		if (put.nget >= nitems)
		{
			Pthread_cond_signal(&nready.cond);
			Pthread_mutex_unlock(&nready.mutex);
			return (NULL);
		}
		nready.nready--;
		j = put.nget++;
		Pthread_mutex_unlock(&nready.mutex);

		if (buff[i] != i)
			printf("buff[%d] = %d\n", i, buff[i]);
	}
	*((int *)arg) -= 1;
	return (NULL);
}
/* end prodcons */
