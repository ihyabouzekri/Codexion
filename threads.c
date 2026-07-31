/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:14:20 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:14:24 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	join_coders(t_data *data, int nbr_coders)
{
	int	i;

	i = 0;
	while (i < nbr_coders)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
}

static void	start_and_join_coders(t_data *data)
{
	data->start_time = get_timenow_ms();
	pthread_mutex_lock(&data->simulation_mutex);
	set_sim_ready(data);
	pthread_mutex_unlock(&data->simulation_mutex);
	pthread_mutex_lock(&data->simulation_mutex);
	pthread_cond_broadcast(&data->ready_cond);
	pthread_mutex_unlock(&data->simulation_mutex);
	join_coders(data, data->nbr_coders);
	pthread_join(data->monitor_thread, NULL);
	clean_up(data);
}

static int	create_coders(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nbr_coders)
	{
		if (pthread_create(&data->coders[i].thread, NULL,
				coder_routine, &data->coders[i]))
		{
			stop_simulation(data);
			pthread_mutex_lock(&data->simulation_mutex);
			set_sim_ready(data);
			pthread_cond_broadcast(&data->ready_cond);
			pthread_mutex_unlock(&data->simulation_mutex);
			pthread_join(data->monitor_thread, NULL);
			join_coders(data, i);
			clean_up(data);
			return (1);
		}
		i++;
	}
	return (0);
}

int	start_simulation(t_data *data)
{
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data))
	{
		clean_up(data);
		return (1);
	}
	if (create_coders(data))
		return (1);
	start_and_join_coders(data);
	return (0);
}
