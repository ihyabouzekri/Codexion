/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:15:32 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:15:33 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongle_objects(t_data *data, t_dongle *dongle, int coder_id)
{
	if (pthread_mutex_init(&dongle->lock, NULL))
	{
		destroy_dongles(data, coder_id);
		return (1);
	}
	return (0);
}

int	init_dongles(t_data *data)
{
	unsigned int	i;

	data->dongles = malloc(sizeof(t_dongle) * data->nbr_coders);
	if (!data->dongles)
		return (1);
	i = 0;
	while (i < data->nbr_coders)
	{
		if (init_dongle_objects(data, &data->dongles[i], i))
			return (1);
		data->dongles[i].available = true;
		data->dongles[i].time_released = 0;
		data->dongles[i].heap.heap_tail = 0;
		if (!data->scheduler)
			data->dongles[i].heap.cmp = cmp_arrival_order;
		else
			data->dongles[i].heap.cmp = cmp_deadline;
		i++;
	}
	return (0);
}
