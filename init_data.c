/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:15:27 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:15:28 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_data_objects(t_data *data)
{
	if (pthread_mutex_init(&data->counter_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->simulation_mutex, NULL))
	{
		pthread_mutex_destroy(&data->counter_mutex);
		return (1);
	}
	if (pthread_mutex_init(&data->log_mutex, NULL))
	{
		pthread_mutex_destroy(&data->simulation_mutex);
		pthread_mutex_destroy(&data->counter_mutex);
		return (1);
	}
	if (pthread_cond_init(&data->ready_cond, NULL))
	{
		pthread_mutex_destroy(&data->log_mutex);
		pthread_mutex_destroy(&data->simulation_mutex);
		pthread_mutex_destroy(&data->counter_mutex);
		return (1);
	}
	return (0);
}

int	init_data(t_data *data)
{
	data->counter = 0;
	data->sim_ready = false;
	data->simul_running = true;
	data->start_time = get_timenow_ms();
	if (init_data_objects(data))
	{
		destroy_dongles(data, data->nbr_coders);
		destroy_coders(data, data->nbr_coders);
		return (1);
	}
	return (0);
}
