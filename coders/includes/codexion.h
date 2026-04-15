#ifndef CODEXION_H
# DEFINE CODEXION_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>


typedef struct s_data 
{
	int				n_coders;
	int				t_burnout;
	int				t_compile;
	int				t_debug;
	int				t_refactor;
	int				n_compile;
	int				dongle_cooldown;
	long long			scheduler;
	int				start_time;
	int				simulation_end_flag;
	pthread_mutex_t			log_lock;
	pthread_mutex_t			end_lock;
	struct	s_dongle	*dongles;
	struct	s_coder		*coders;
}	t_data;


typedef struct s_dongle
{
	pthread_mutex_t	lock;
	pthread_mutex_t cond;
	long long	available_at; // timesstamp apos cooldown
	t_heap		*queue;  // fila de prioridade
	int		is_taken;
}	t_dongle;


typedef struct s_coder
{
	pthread_t	thread;
	int		id;
	int		comp_done; // qunatas vezes ja compilou
	long long	last_comp;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_data		*data;
}	t_coder;

#endif
