/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:26:23 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:17:32 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define FIRST_IN_FIRST_OUT 0
# define EARLIEST_DEADLINE_FIRST 1
# define MAX_INPUT_VALUE 2147483647LL

typedef struct s_simulation	t_simulation;

typedef struct s_waiting_coder
{
	int			coder_id;
	long long	priority_key;
	long long	sequence;
}t_waiting_coder;

typedef struct s_priority_queue
{
	t_waiting_coder	*entries;
	int				count;
	int				capacity;
}t_priority_queue;

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	pthread_cond_t	available;
	int				held;
	long long		available_after_ms;
	long long		next_sequence;
	t_priority_queue	queue;
}t_dongle;

typedef struct s_coder
{
	pthread_t		thread;
	pthread_mutex_t	state_lock;
	int				id;
	int				times_compiled;
	long long		last_compile_start;
	t_dongle		*left;
	t_dongle		*right;
	t_simulation	*sim;
}t_coder;

typedef struct s_simulation
{
	int				number_of_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				compiles_required;
	long long		dongle_cooldown;
	int				scheduler;
	long long		start_time;
	int				stop;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	start_lock;
	pthread_cond_t	start_cond;
	int				start_released;
	pthread_t		monitor_thread;
	int				monitor_created;
	int				threads_created;
	int				dongles_initialized;
	int				coders_initialized;
	int				base_mutexes_initialized;
	t_dongle		*dongles;
	t_coder			*coders;
}t_simulation;

int			parse_arguments(t_simulation *sim, int argc, char **argv);
int			parse_positive(const char *str, long long *value, int allow_zero);
int			simulation_init(t_simulation *sim, int argc, char **argv);
int			simulation_run(t_simulation *sim);
void			simulation_destroy(t_simulation *sim);
int			simulation_stopped(t_simulation *sim);
int			simulation_stop(t_simulation *sim);
void			wake_all_dongles(t_simulation *sim);
void			wait_for_start(t_simulation *sim);

long long	get_time_ms(void);
long long	time_since_start(t_simulation *sim);
int			smart_sleep(t_simulation *sim, long long duration_ms);
void			make_timeout(struct timespec *ts, long long wait_ms);

void			print_status(t_coder *coder, const char *message);
void			print_burnout(t_coder *coder);

int			pqueue_init(t_priority_queue *pq, int capacity);
void			pqueue_free(t_priority_queue *pq);
int			pqueue_push(t_priority_queue *pq, t_waiting_coder entry);
int			pqueue_pop(t_priority_queue *pq, t_waiting_coder *entry);
int			pqueue_remove(t_priority_queue *pq, int coder_id);
int			pqueue_is_empty(t_priority_queue *pq);
int			waiter_before(t_waiting_coder a, t_waiting_coder b);
void			waiter_swap(t_waiting_coder *a, t_waiting_coder *b);

int			dongle_init(t_dongle *dongle, int capacity);
void			dongle_destroy(t_dongle *dongle);
int			dongle_pick_up(t_dongle *dongle, t_coder *coder);
void			dongle_put_down(t_dongle *dongle, t_simulation *sim);

void			*coder_life_cycle(void *arg);
void			*burnout_monitor(void *arg);
long long	coder_last_compile(t_coder *coder);
int			coder_compile_count(t_coder *coder);
void			coder_mark_compile_start(t_coder *coder);
void			coder_increment_compile_count(t_coder *coder);

#endif

