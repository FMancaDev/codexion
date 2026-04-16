#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>


// constantes scheduler
# define FIRST_IN_FIRST_OUT		0
# define EARLIEST_DEADLINE_FIRST	1


// fila de propriedades
typedef struct s_waiting_coder
{
	int			coder_id;
	long long		priority_key;
}	t_waiting_coder;


typedef struct s_priority_queue
{
	t_waiting_coder	*entries;
	int			count;
	int			max_capacity;
}	t_priority_queue;



typedef struct s_dongle
{
	pthread_mutex_t		access_lock;
	pthread_cond_t		became_available;
	int			is_held;
	long long		available_after_ms; // timestamp apos cooldown
	t_priority_queue	waiting_coders; // fila de propriedades
}	t_dongle;

typedef struct s_coder
{
	pthread_t		thread;
	int			id;
	int			times_compiled;
	long long		last_compile_started_at;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_simulation		*simulation;
}	t_coder;

typedef struct s_simulation
{
	int			number_of_coders;
	long long		time_to_burnout_ms;
	long long		time_to_compile_ms;
	long long		time_to_debug_ms;
	long long		time_to_refactor_ms;
	int			compiles_required_to_stop;
	long long		dongle_cooldown_ms;
	int			scheduler_mode;
	long long		simulation_started_at;
	int			simulation_should_stop;
	pthread_mutex_t		print_lock;
	pthread_mutex_t		stop_flag_lock;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_simulation;


// gestao de tempo e logs
long long	get_time_ms(void);
long long	time_since_start(t_simulation *sim);
void		print_coder_status(t_coder *coder, const char *status_message);
void		print_burnout(t_coder *coder);


// prioridade - heap
int			pqueue_init(t_priority_queue *pq, int capacity);
void			pqueue_free(t_priority_queue *pq);
int			pqueue_push(t_priority_queue *pq, int coder_id, long long key);
t_waiting_coder		pqueue_pop(t_priority_queue *pq);
int			pqueue_is_empty(t_priority_queue *pq);


// operacoes dos dongles
int		dongle_init(t_dongle *dongle, int max_waiting_coders);
void		dongle_destroy(t_dongle *dongle);
void		dongle_pick_up(t_dongle *dongle, t_coder *coder);
void		dongle_put_down(t_dongle *dongle, t_simulation *sim);


// rotinas das threads
void		*coder_life_cycle(void *coder_ptr);
void		*burnout_monitor(void *simulation_ptr);


// cleanup
int		simulation_init(t_simulation *sim, int argc, char **argv);
void		simulation_destroy(t_simulation *sim);

#endif
