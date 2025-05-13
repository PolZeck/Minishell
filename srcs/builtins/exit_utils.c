/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:42:38 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/13 15:34:43 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	print_numeric_error(char *arg)
{
	ft_putstr_fd("bash: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	*get_exit_status() = 2;
}

int	is_strict_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	handle_invalid_argument(t_cmd *cmd, t_data *data)
{
	print_numeric_error(cmd->args[1]);
	exit_cleanup(cmd, data, 2);
}

void	handle_too_many_arguments(void)
{
	ft_putstr_fd("bash: exit: too many arguments\n", 2);
	*get_exit_status() = 1;
}

int	str_to_llong(const char *str, long long *out)
{
	long long	res;
	int			sign;
	int			i;
	int			digit;

	i = 0;
	sign = 1;
	res = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		digit = str[i] - '0';
		if ((sign == 1 && res > (LLONG_MAX - digit) / 10)
			|| (sign == -1 && (-res) < (LLONG_MIN + digit) / 10))
			return (0);
		res = res * 10 + digit;
		i++;
	}
	*out = res * sign;
	return (1);
}
