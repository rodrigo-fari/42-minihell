/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:11:14 by aeberius          #+#    #+#             */
/*   Updated: 2025/05/29 02:25:30 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_print_fd(char *str, int fd)
{
	write(fd, str, ft_strlen(str));
}

int	count_args(char **commands)
{
	int	count;

	count = 0;
	while (commands[count])
		count++;
	return (count);
}

void	update_shell_level(void)
{
	t_env	*env;
	int		value_to_int;
	char	*value_to_char;

	value_to_int = 0;
	env = get_env(NULL);
	while (env)
	{
		if (ft_strcmp(env->key, "SHLVL") == 0)
		{
			value_to_int = ft_atoi(env->value);
			value_to_int++;
			value_to_char = ft_itoa(value_to_int);
			env_update(env, "SHLVL", value_to_char, true);
		}
		env = env->next;
	}
}

void	shlvl_warning(void)
{
	t_env	*env;
	t_env	*tmp;

	env = get_env(NULL);
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "SHLVL") == 0
			&& ft_strcmp(tmp->value, "1000") == 0)
		{
			print_error("Minishell: ");
			print_error("warning: ");
			print_error("shell level (1000) too high, resetting to 1");
			env_update(tmp, "SHLVL", ft_itoa(1), true);
		}
		tmp = tmp->next;
	}
}
