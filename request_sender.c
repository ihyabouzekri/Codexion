/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_sender.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:14:36 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:14:37 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	cmp_deadline(t_request	req1, t_request	req2)
{
	if (req1.coder_deadline > req2.coder_deadline)
		return (1);
	else if (req1.coder_deadline == req2.coder_deadline)
	{
		if (req1.coder->compiles_done > req2.coder->compiles_done)
			return (1);
		else if (req1.coder->coder_id > req2.coder->coder_id)
			return (1);
	}
	return (0);
}

bool	cmp_arrival_order(t_request	req1, t_request	req2)
{
	if (req1.arrival_num > req2.arrival_num)
		return (1);
	return (0);
}

static unsigned int	get_arrival_num(t_data *data)
{
	unsigned int	order_num;

	pthread_mutex_lock(&data->counter_mutex);
	order_num = data->counter++;
	pthread_mutex_unlock(&data->counter_mutex);
	return (order_num);
}

void	request(t_dongle *dongle, t_request req)
{
	pthread_mutex_lock(&dongle->lock);
	heap_push(&dongle->heap, req);
	pthread_mutex_unlock(&dongle->lock);
}

void	send_requests(t_coder *coder)
{
	t_request	req;
	long		last_compile;

	pthread_mutex_lock(&coder->mutex);
	last_compile = coder->last_compile;
	pthread_mutex_unlock(&coder->mutex);
	req.coder = coder;
	req.arrival_num = get_arrival_num(coder->data);
	req.coder_deadline = last_compile + coder->data->burnout_time;
	coder->request = req;
	request(coder->left_dong, req);
	request(coder->right_dong, req);
}
