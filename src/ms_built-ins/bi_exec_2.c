/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exec_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:41:39 by aeberius          #+#    #+#             */
/*   Updated: 2025/05/28 17:40:12 by rde-fari         ###   ########.fr       */
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

void	bi_exec(char **commands, t_env *env)
{
	char	**splitted_envs;
	char	*command_path;
	t_shell	*shell;

	command_path = NULL;
	splitted_envs = NULL;
	if (ft_strcmp(commands[0], "") == 0 && !commands[1])
	{
		cc_shell(get_shell(), true, true, false);
		return ;
	}
	shell = get_shell();
	env = get_env(NULL);
	handle_builtin_or_empty(commands, env);
	command_path = resolve_command_path(commands[0], env);
	if (!command_path)
		handle_command_not_found(commands[0], shell);
	check_command_path(command_path, commands, shell);
	if (ft_strcmp(commands[0], "") != 0)
	{
		splitted_envs = array_envs(env);
		execve(command_path, commands, splitted_envs);
	}
	free(command_path);
	free_splits(splitted_envs);
	cc_shell(shell, true, true, false);
	exit (g_exit_status);
}

void	execute_builtin(char **commands, t_env *env, t_token *tokens)
{
	t_token	*token;

	env = get_env(NULL);
	if (ft_strcmp(commands[0], "echo") == 0)
	{
		token = token_to_struct(commands);
		bi_echo(token);
		free_tokens(token);
	}
	else if (ft_strcmp(commands[0], "pwd") == 0)
		bi_pwd();
	else if (ft_strcmp(commands[0], "exit") == 0)
		bi_exit(tokens);
	else if (ft_strcmp(commands[0], "env") == 0)
		print_env(env);
	else if (ft_strcmp(commands[0], "cd") == 0)
		bi_cd(commands, env);
	else if (ft_strcmp(commands[0], "unset") == 0)
		bi_unset(commands, env);
	else if (ft_strcmp(commands[0], "export") == 0)
		bi_export(env, commands);
}
