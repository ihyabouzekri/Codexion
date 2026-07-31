/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:16:12 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:16:13 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

# define INT_MAX 2147483647

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;
typedef struct timeval	t_timeval;

typedef struct s_request
{
	t_coder			*coder;
	long			coder_deadline;
	unsigned int	arrival_num;
}	t_request;

typedef struct s_heap
{
	t_request		requests[2];
	unsigned int	heap_tail;
	bool			(*cmp)(t_request, t_request);
}	t_heap;

typedef struct s_dongle
{
	bool				available;
	long				time_released;
	t_heap				heap;
	pthread_mutex_t		lock;

}	t_dongle;

// global data container
typedef struct s_data
{
	// config
	unsigned int		nbr_coders;
	unsigned int		burnout_time;
	unsigned int		compile_time;
	unsigned int		debug_time;
	unsigned int		refact_time;
	unsigned int		nb_compilation;
	unsigned int		dongle_cooldown;
	int					scheduler;

	// simulation data
	long				start_time;
	bool				simul_running;
	bool				sim_ready;
	t_coder				*coders;
	t_dongle			*dongles;
	pthread_t			monitor_thread;
	unsigned int		counter;
	pthread_mutex_t		counter_mutex;
	pthread_mutex_t		simulation_mutex;
	pthread_mutex_t		log_mutex;
	pthread_cond_t		ready_cond;

}	t_data;

typedef struct s_coder
{
	unsigned int		coder_id;
	pthread_t			thread;
	unsigned int		compiles_done;
	t_dongle			*left_dong;
	t_dongle			*right_dong;
	long				last_compile;
	t_request			request;
	pthread_mutex_t		mutex;
	t_data				*data;

}	t_coder;

//parsing
unsigned int	parse_args(int argc, char *argv[], int user_input[]);
unsigned int	setup_config(int parsed_input[], t_data *data);

// init structs + destroy
int				init_data(t_data *data);
void			destroy_data(t_data *data);
int				init_dongles(t_data *data);
void			destroy_dongles(t_data *data, int nbr_coders);
int				init_coders(t_data *data);
void			destroy_coders(t_data *data, int nbr_coders);

// simulation utils
long			get_timenow_ms(void);
bool			sim_running(t_data *data);
void			stop_simulation(t_data *data);
bool			sim_ready(t_data *data);
void			set_sim_ready(t_data *data);
void			ft_sleep(long time_to_sleep, t_data *data);
void			print_log(t_data *data, unsigned int coder_id, char *str);
void			clean_up(t_data *data);

// dongles taking and releasing
void			send_requests(t_coder *coder);
void			request(t_dongle *dongle, t_request req);
void			release_dongles(t_coder *coder);
int				take_dongles(t_coder *coder);
bool			is_dongle_available(t_dongle *dongle);
bool			is_my_turn(unsigned int coder_id, t_heap *heap);
bool			cooldown_passed(t_dongle *dongle, unsigned int dongle_cooldown);

// coder routine + monitor routine + thread creating
int				start_simulation(t_data *data);
void			*coder_routine(void *arg);
void			*monitor_routine(void *arg);

// heap funcs
void			heap_push(t_heap *heap, t_request req);
unsigned int	heap_peek(t_heap	*heap);
t_request		heap_pop(t_heap *heap);
void			ft_swap(t_request *req1, t_request *req2);

// cmp funcs
bool			cmp_deadline(t_request	req1, t_request	req2);
bool			cmp_arrival_order(t_request	req1, t_request	req2);

#endif