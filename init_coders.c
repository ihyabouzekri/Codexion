/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:15:32 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:15:33 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_coder_structs(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nbr_coders)
	{
		if (pthread_mutex_init(&data->coders[i].mutex, NULL))
		{
			destroy_dongles(data, data->nbr_coders);
			destroy_coders(data, i);
			return (1);
		}
		data->coders[i].coder_id = i + 1;
		data->coders[i].compiles_done = 0;
		data->coders[i].last_compile = get_timenow_ms();
		data->coders[i].left_dong = &data->dongles[i];
		data->coders[i].right_dong = &data->dongles[
			(i + 1) % (data->nbr_coders)];
		data->coders[i].data = data;
		i++;
	}
	return (0);
}

int	init_coders(t_data *data)
{
	data->coders = malloc(data->nbr_coders * sizeof(t_coder));
	if (!data->coders)
	{
		destroy_dongles(data, data->nbr_coders);
		return (1);
	}
	if (init_coder_structs(data))
		return (1);
	return (0);
}
