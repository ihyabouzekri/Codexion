/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:15:47 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:15:48 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	try_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	if (!is_my_turn(coder->coder_id, &dongle->heap)
		|| !is_dongle_available(dongle)
		|| !cooldown_passed(dongle, coder->data->dongle_cooldown))
	{
		pthread_mutex_unlock(&dongle->lock);
		usleep(250);
		return (1);
	}
	dongle->available = false;
	heap_pop(&dongle->heap);
	pthread_mutex_unlock(&dongle->lock);
	return (0);
}

static void	give_back_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->available = true;
	heap_push(&dongle->heap, coder->request);
	pthread_mutex_unlock(&dongle->lock);
	usleep(250);
}

static void	pick_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->coder_id % 2 == 0)
	{
		*first = coder->right_dong;
		*second = coder->left_dong;
	}
	else
	{
		*first = coder->left_dong;
		*second = coder->right_dong;
	}
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	send_requests(coder);
	pick_order(coder, &first, &second);
	while (1)
	{
		if (!sim_running(coder->data))
			return (1);
		if (try_dongle(coder, first))
			continue ;
		if (!try_dongle(coder, second))
			break ;
		give_back_dongle(coder, first);
	}
	print_log(coder->data, coder->coder_id, "has taken a dongle");
	print_log(coder->data, coder->coder_id, "has taken a dongle");
	return (0);
}
