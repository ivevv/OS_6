//Иванчук Евгения 9894

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctime>
#include <sys/mman.h>
#include <cstring>

using namespace std;

sem_t semR, semW;
bool flag = true;
int *ptr;

void *func(void *arg)
{
	int val = 0;
	while (flag == 1)
	{
		srand(time(NULL));
		val = rand() % 100;
		cout << "value in 6_1: " << val << endl;
		cout.flush();
		memcpy(ptr, &val, sizeof(int));
		sem_post(&semW);
		sem_wait(&semR);
		sleep(1);
	}
	return NULL;
}

int main()
{
	int shm = shm_open("/shm", O_CREAT|O_RDWR, 0644);
	ftruncate(shm, sizeof(int));
	ptr = (int*)mmap(0, sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
	sem_open("/semR", O_CREAT, 0644, 0);
	sem_open("/semW", O_CREAT, 0644, 0);
	pthread_t th;
	pthread_create(&th, NULL, func, NULL);
	getchar();
	flag = false;
	pthread_join(th, NULL);
	sem_close(&semR);
	sem_unlink("/semR");
	sem_close(&semW);
	sem_unlink("/semW");
	munmap(ptr, sizeof(int));
	close(shm);
	shm_unlink("/shm");
	return 0;
}
