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

int	ft_atoi(const char *arg)
{
	int	res;

	res = 0;
	if (*arg == '+')
		arg++;
	if (!(*arg))
		return (-1);
	while ('0' <= *arg && *arg <= '9')
	{
		if (res > INT_MAX / 10
			|| (res == INT_MAX / 10
				&& (*arg - '0') > INT_MAX % 10))
			return (-1);
		res = res * 10 + (*arg - '0');
		arg++;
	}
	if (*arg)
		return (-1);
	return (res);
}

unsigned int	setup_config(int parsed_input[], t_data *data)
{
	if (!parsed_input[0])
	{
		fprintf(stderr, "number_of_coders must be greater than 0");
		return (1);
	}
	data->nbr_coders = parsed_input[0];
	data->burnout_time = parsed_input[1];
	data->compile_time = parsed_input[2];
	data->debug_time = parsed_input[3];
	data->refact_time = parsed_input[4];
	data->nb_compilation = parsed_input[5];
	data->dongle_cooldown = parsed_input[6];
	data->scheduler = parsed_input[7];
	return (0);
}

unsigned int	parse_args(int argc, char *argv[], int user_input[])
{
	int	i;

	i = 0;
	if (argc != 9)
		return (1);
	while (++i < argc - 1)
	{
		user_input[i - 1] = ft_atoi(argv[i]);
		if (user_input[i - 1] < 0)
			return (1);
	}
	if (!strcmp(argv[8], "fifo"))
		user_input[7] = 0;
	else if (!strcmp(argv[8], "edf"))
		user_input[7] = 1;
	else
		return (1);
	return (0);
}
