/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:15:12 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:15:13 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

unsigned int	heap_peek(t_heap	*heap)
{
	return (heap->requests[0].coder->coder_id);
}

void	heap_push(t_heap *heap, t_request req)
{
	int	curr_position;
	int	parent;

	curr_position = heap->heap_tail++;
	heap->requests[curr_position] = req;
	parent = (curr_position - 1) / 2;
	while (parent >= 0)
	{
		if (heap->cmp(heap->requests[parent], heap->requests[curr_position]))
			ft_swap(&heap->requests[parent], &heap->requests[curr_position]);
		else
			break ;
		curr_position = parent;
		parent = (curr_position - 1) / 2;
	}
}

void	sift_down(t_heap *heap)
{
	unsigned int	posi;
	unsigned int	left_idx;
	unsigned int	right_idx;
	unsigned int	target;

	posi = 0;
	while ((posi * 2 + 1) < heap->heap_tail)
	{
		left_idx = (posi * 2) + 1;
		right_idx = (posi * 2) + 2;
		target = left_idx;
		if (right_idx < heap->heap_tail
			&& heap->cmp(heap->requests[left_idx], heap->requests[right_idx]))
			target = right_idx;
		if (heap->cmp(heap->requests[posi], heap->requests[target]))
		{
			ft_swap(&heap->requests[posi], &heap->requests[target]);
			posi = target;
		}
		else
			break ;
	}
}

t_request	heap_pop(t_heap *heap)
{
	t_request		holder;

	holder = heap->requests[0];
	heap->requests[0] = heap->requests[heap->heap_tail - 1];
	heap->heap_tail--;
	sift_down(heap);
	return (holder);
}
