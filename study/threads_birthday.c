#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* rotina_thread(void *arg)
{
	int id = *(int*)arg;

	printf("Boas!, eu sou a thread numero: %d\n", id);
	return NULL;
}

int main()
{
	pthread_t threads[5];
	int ids[5]; // array de 1 a 5
	int i;

	i = 0;

	while (i < 5)
	{
		ids[i] = i + 1;
		if (pthread_create(&threads[i], NULL, rotina_thread, &ids[i]) != 0)
		{
			printf("Erro ao criar a thread: %d\n", i);
		}
		i++;
	}
	
	i = 0;
	while (i < 5)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	printf("\nTodas as threads terminaram\n");
	return 0;
}

// cria a thread
// atribuo null de padrao
// a funcao que vai executar
// e o argumento que queremos passar(id)
