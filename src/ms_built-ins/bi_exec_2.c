/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exec_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:41:39 by aeberius          #+#    #+#             */
/*   Updated: 2025/05/29 18:41:16 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_directory_error(char *command)
{
	t_shell	*shell;

	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": is a directory\n", STDERR_FILENO);
	g_exit_status = 126;
	shell = get_shell();
	cc_shell(shell, true, true, false);
	exit(g_exit_status);
}

void	check_command_path(char *command_path, char **commands, t_shell *shell)
{
	struct stat	file_stat;

	if (stat(command_path, &file_stat) == -1
		|| access(command_path, X_OK) == -1)
		handle_command_not_found(commands[0], shell);
}

bool	verify_commands(char **commands)
{
	if (ft_strcmp(commands[0], "") == 0 && !commands[1])
	{
		cc_shell(get_shell(), true, true, false);
		return (false);
	}
	return (true);
}

void	bi_exec(char **commands, t_env *env)
{
	char	**splitted_envs;
	char	*command_path;
	char	**new_commands;
	t_shell	*shell;

	command_path = NULL;
	splitted_envs = NULL;
	new_commands = prepare_exec_commands(commands);
	if (!verify_commands(new_commands))
	{
		free_splits(new_commands);
		return ;
	}
	shell = get_shell();
	env = get_env(NULL);
	handle_builtin_or_empty(new_commands, env);
	command_path = resolve_command_path(new_commands[0], env);
	if (!command_path)
	{
		free_splits(new_commands);
		handle_command_not_found(commands[0], shell);
	}
	check_command_path(command_path, new_commands, shell);
	if (ft_strcmp(new_commands[0], "") != 0)
	{
		splitted_envs = array_envs(env);
		execve(command_path, new_commands, splitted_envs);
	}
	free(command_path);
	free_splits(splitted_envs);
	free_splits(new_commands);
	free_splits(commands);
	cc_shell(shell, true, true, false);
	exit (g_exit_status);
}

char	**prepare_builtin_commands(char **commands)
{
	char	**split_cmd;
	char	**new_commands;
	int		i;
	int		j;

	split_cmd = ft_split(commands[0], ' ');
	if (!split_cmd || !split_cmd[0])
	{
		free_splits(split_cmd);
		return (NULL);
	}
	for (i = 0; split_cmd[i]; i++);
	for (j = 1; commands[j]; j++);
	new_commands = malloc((i + j + 1) * sizeof(char *));
	if (!new_commands)
	{
		free_splits(split_cmd);
		return (NULL);
	}
	for (i = 0; split_cmd[i]; i++)
		new_commands[i] = ft_strdup(split_cmd[i]);
	for (j = 1; commands[j]; j++)
		new_commands[i++] = ft_strdup(commands[j]);
	new_commands[i] = NULL;
	free_splits(split_cmd);
	return (new_commands);
}

void	execute_builtin(char **commands, t_env *env, t_token *tokens)
{
	t_token	*token;
	char	**new_commands;

	env = get_env(NULL);
	new_commands = prepare_builtin_commands(commands);
	if (!new_commands)
		return ;
	if (ft_strcmp(new_commands[0], "echo") == 0)
	{
		token = token_to_struct(new_commands, 0);
		bi_echo(token);
		free_tokens(token);
	}
	else if (ft_strcmp(new_commands[0], "pwd") == 0)
		bi_pwd();
	else if (ft_strcmp(commands[0], "exit") == 0)
		bi_exit(tokens, new_commands);
	else if (ft_strcmp(new_commands[0], "env") == 0)
		print_env(env);
	else if (ft_strcmp(new_commands[0], "cd") == 0)
		bi_cd(new_commands, env);
	else if (ft_strcmp(new_commands[0], "unset") == 0)
		bi_unset(new_commands, env);
	else if (ft_strcmp(new_commands[0], "export") == 0)
		bi_export(env, new_commands);
}

 char	**prepare_exec_commands(char **commands)
{
	char	**split_cmd;
	char	**new_commands;
	int		i, j;

	split_cmd = ft_split(commands[0], ' ');
	if (!split_cmd || !split_cmd[0])
	{
		free_splits(split_cmd);
		return (NULL);
	}
	for (i = 0; split_cmd[i]; i++);
	for (j = 1; commands[j]; j++);
	new_commands = malloc((i + j + 1) * sizeof(char *));
	if (!new_commands)
	{
		free_splits(split_cmd);
		return (NULL);
	}
	for (i = 0; split_cmd[i]; i++)
		new_commands[i] = ft_strdup(split_cmd[i]);
	for (j = 1; commands[j]; j++)
		new_commands[i++] = ft_strdup(commands[j]);
	new_commands[i] = NULL;
	free_splits(split_cmd);
	return (new_commands);
}
