/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 13:23:49 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/30 18:04:42 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_split_cmd(char **dst, char **split_cmd, int *j)
{
	int	i;

	i = 0;
	while (split_cmd[i])
	{
		if (split_cmd[i][0] != '\0')
			dst[(*j)++] = ft_strdup(split_cmd[i]);
		i++;
	}
}

static void	copy_rest_commands(char **dst, char **commands, int *j)
{
	int	i;

	i = 1;
	while (commands[i])
	{
		if (commands[i][0] != '\0')
			dst[(*j)++] = ft_strdup(commands[i]);
		i++;
	}
}

char	**prepare_builtin_commands(char **commands)
{
	char	**split_cmd;
	char	**new_commands;
	int		j;

	split_cmd = ft_split(commands[0], ' ');
	if (!split_cmd || !split_cmd[0])
		return (free_splits(split_cmd), NULL);
	new_commands = ft_calloc(sizeof(char *),
			(ft_array_size(split_cmd) + ft_array_size(commands) + 1));
	if (!new_commands)
		return (free_splits(split_cmd), NULL);
	j = 0;
	copy_split_cmd(new_commands, split_cmd, &j);
	copy_rest_commands(new_commands, commands, &j);
	new_commands[j] = NULL;
	return (free_splits(split_cmd), new_commands);
}

bool	verify_commands(char **commands)
{
	if (!commands)
		return (false);
	if (ft_strcmp(commands[0], "") == 0 && !commands[1])
	{
		cc_shell(get_shell(), true, true, false);
		return (false);
	}
	return (true);
}

void	execute_cmds(char **new_commands, char *command_path, char **commands)
{
	char	**splitted_envs;
	t_env	*env;

	splitted_envs = NULL;
	env = get_env(NULL);
	if (ft_strcmp(new_commands[0], "") != 0)
	{
		splitted_envs = array_envs(env);
		execve(command_path, new_commands, splitted_envs);
	}
	free(command_path);
	if (splitted_envs)
		free_splits(splitted_envs);
	free_splits(new_commands);
	free_splits(commands);
	cc_shell(get_shell(), true, true, false);
	return ;
}
