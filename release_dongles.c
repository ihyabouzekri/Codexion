/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:14:46 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:14:47 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	update_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->available = true;
	dongle->time_released = get_timenow_ms();
	pthread_mutex_unlock(&dongle->lock);
}

void	release_dongles(t_coder *coder)
{
	update_dongle(coder->left_dong);
	update_dongle(coder->right_dong);
}
