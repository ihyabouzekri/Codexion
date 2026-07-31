/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:14:29 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:14:31 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	sim_running(t_data *data)
{
	bool	sim_state;

	pthread_mutex_lock(&data->simulation_mutex);
	sim_state = data->simul_running;
	pthread_mutex_unlock(&data->simulation_mutex);
	return (sim_state);
}

void	stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->simulation_mutex);
	data->simul_running = false;
	pthread_mutex_unlock(&data->simulation_mutex);
}

void	set_sim_ready(t_data *data)
{
	data->sim_ready = true;
}

bool	sim_ready(t_data *data)
{
	return (data->sim_ready);
}
