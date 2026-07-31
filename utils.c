/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:14:04 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:14:07 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_timenow_ms(void)
{
	t_timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_log(t_data *data, unsigned int coder_id, char *str)
{
	long	timestamp;

	pthread_mutex_lock(&data->log_mutex);
	timestamp = get_timenow_ms() - data->start_time;
	if (sim_running(data))
		printf("%ld %i %s\n", timestamp, coder_id, str);
	pthread_mutex_unlock(&data->log_mutex);
}

void	ft_sleep(long time_to_sleep, t_data *data)
{
	long	start_time;

	start_time = get_timenow_ms();
	while (get_timenow_ms() - start_time < time_to_sleep)
	{
		if (!sim_running(data))
			return ;
		usleep(500);
	}
}

void	clean_up(t_data *data)
{
	destroy_dongles(data, data->nbr_coders);
	destroy_coders(data, data->nbr_coders);
	destroy_data(data);
}

void	ft_swap(t_request *req1, t_request *req2)
{
	t_request	temp_req;

	temp_req = *req1;
	*req1 = *req2;
	*req2 = temp_req;
}
