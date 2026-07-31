/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:15:07 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:15:08 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_data *data)
{
	unsigned int	i;
	long			curr_time;
	long			timestamp;
	long			coder_last_compile;

	i = 0;
	curr_time = get_timenow_ms();
	while (i < data->nbr_coders)
	{
		pthread_mutex_lock(&data->coders[i].mutex);
		coder_last_compile = data->coders[i].last_compile;
		pthread_mutex_unlock(&data->coders[i].mutex);
		if (data->burnout_time < (curr_time - coder_last_compile))
		{
			pthread_mutex_lock(&data->log_mutex);
			timestamp = get_timenow_ms() - data->start_time;
			stop_simulation(data);
			printf("%ld %i %s\n", timestamp, i + 1, "burned out");
			pthread_mutex_unlock(&data->log_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	is_all_finished(t_data *data)
{
	unsigned int	counter;
	unsigned int	compiles_done;
	unsigned int	i;

	counter = 0;
	i = 0;
	while (i < data->nbr_coders)
	{
		pthread_mutex_lock(&data->coders[i].mutex);
		compiles_done = data->coders[i].compiles_done;
		pthread_mutex_unlock(&data->coders[i].mutex);
		if (compiles_done >= data->nb_compilation)
			counter++;
		i++;
	}
	if (counter == data->nbr_coders)
	{
		stop_simulation(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	pthread_mutex_lock(&data->simulation_mutex);
	while (!sim_ready(data))
		pthread_cond_wait(&data->ready_cond, &data->simulation_mutex);
	pthread_mutex_unlock(&data->simulation_mutex);
	if (!sim_running(data))
		return (NULL);
	while (1)
	{
		usleep(500);
		if (check_burnout(data))
			break ;
		if (is_all_finished(data))
			break ;
	}
	return (NULL);
}
