/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:15:19 by ibouzekr          #+#    #+#             */
/*   Updated: 2026/07/12 21:15:20 by ibouzekr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char *argv[])
{
	t_data	data;
	int		user_input[8];

	if (parse_args(argc, argv, user_input))
	{
		fprintf(stderr, "invalid args\n");
		return (1);
	}
	if (setup_config(user_input, &data))
		return (1);
	if (!data.nb_compilation)
		return (0);
	if (init_dongles(&data)
		|| init_coders(&data)
		|| init_data(&data))
	{
		fprintf(stderr, "Error: insufficient resources\n");
		return (1);
	}
	if (start_simulation(&data))
	{
		fprintf(stderr, "Error: insufficient resources to create threads\n");
		return (1);
	}
	return (0);
}
