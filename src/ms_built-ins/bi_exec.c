/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:18:54 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/30 16:14:24 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_direct_path(const char *cmd)
{
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

char	*find_executable(char **paths, const char *cmd)
{
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*resolve_command_path(const char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*result;

	(void)env;
	if (ft_strcmp((char *)cmd, "./minishell") == 0)
		update_shell_level();
	result = check_direct_path(cmd);
	if (result)
		return (result);
	path_env = get_own_env("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = find_executable(paths, cmd);
	free_splits(paths);
	return (result);
}

void	handle_builtin_or_empty(char **commands, t_env *env)
{
	t_shell	*shell;

	shell = get_shell();
	if (!commands)
	{
		g_exit_status = 0;
		return ;
	}
	while (commands[0] && commands[0][0] == '\0')
		commands++;
	if (!commands[0] || commands[0][0] == '\0')
	{
		g_exit_status = 0;
		return ;
	}
	if (is_builtin(commands[0]))
	{
		execute_builtin(commands, env, NULL);
		free_splits(commands);
		cc_shell(shell, true, true, false);
		exit(0);
	}
}

void	handle_command_not_found(char *command, t_shell *shell,
			char **new_commands)
{
	free_splits(new_commands);
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	g_exit_status = 127;
	cc_shell(shell, true, true, false);
	exit(g_exit_status);
}
