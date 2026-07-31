/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ressources.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:15:32 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:15:33 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_dongles(t_data *data, int nbr_coders)
{
	int	i;

	i = 0;
	while (i < nbr_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].lock);
		i++;
	}
	free(data->dongles);
}

void	destroy_coders(t_data *data, int nbr_coders)
{
	int	i;

	i = 0;
	while (i < nbr_coders)
	{
		pthread_mutex_destroy(&data->coders[i].mutex);
		i++;
	}
	free(data->coders);
}

void	destroy_data(t_data *data)
{
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->simulation_mutex);
	pthread_mutex_destroy(&data->counter_mutex);
	pthread_cond_destroy(&data->ready_cond);
}
