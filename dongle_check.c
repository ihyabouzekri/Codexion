/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:15:51 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:15:52 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// cooldown + priority + available
bool	cooldown_passed(t_dongle *dongle, unsigned int dongle_cooldown)
{
	if (dongle->time_released == 0)
		return (1);
	return (get_timenow_ms() >= dongle->time_released + dongle_cooldown);
}

bool	is_my_turn(unsigned int coder_id, t_heap *heap)
{
	return (heap_peek(heap) == coder_id);
}

bool	is_dongle_available(t_dongle *dongle)
{
	return (dongle->available);
}
