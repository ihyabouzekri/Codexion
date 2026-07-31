/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:17:15 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:17:16 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	wait_sim_start(t_coder *coder, t_data *data)
{
	pthread_mutex_lock(&data->simulation_mutex);
	while (!sim_ready(data))
		pthread_cond_wait(&data->ready_cond, &data->simulation_mutex);
	pthread_mutex_unlock(&data->simulation_mutex);
	if (!sim_running(data))
		return (1);
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile = data->start_time;
	pthread_mutex_unlock(&coder->mutex);
	if (data->nbr_coders == 1)
	{
		print_log(data, coder->coder_id, "has taken a dongle");
		return (1);
	}
	if (coder->coder_id % 2 == 0)
		ft_sleep(((data->compile_time + data->dongle_cooldown) / 2), data);
	return (0);
}

static void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile = get_timenow_ms();
	pthread_mutex_unlock(&coder->mutex);
	print_log(coder->data, coder->coder_id, "is compiling");
	ft_sleep(coder->data->compile_time, coder->data);
	pthread_mutex_lock(&coder->mutex);
	coder->compiles_done += 1;
	pthread_mutex_unlock(&coder->mutex);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (wait_sim_start(coder, coder->data))
		return (NULL);
	while (1)
	{
		if (take_dongles(coder))
			return (NULL);
		compile(coder);
		release_dongles(coder);
		if (!sim_running(coder->data))
			return (NULL);
		print_log(coder->data, coder->coder_id, "is debugging");
		ft_sleep(coder->data->debug_time, coder->data);
		if (!sim_running(coder->data))
			return (NULL);
		print_log(coder->data, coder->coder_id, "is refactoring");
		ft_sleep(coder->data->refact_time, coder->data);
		if (!sim_running(coder->data))
			return (NULL);
	}
	return (NULL);
}
