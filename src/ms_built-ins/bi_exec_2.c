/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exec_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:41:39 by aeberius          #+#    #+#             */
/*   Updated: 2025/05/31 13:52:56 by rde-fari         ###   ########.fr       */
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
		handle_command_not_found(commands[0], shell, commands);
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
		handle_command_not_found(commands[0], shell, new_commands);
	check_command_path(command_path, new_commands, shell);
	execute_cmds(new_commands, command_path, commands);
	exit (g_exit_status);
}

void	execute_builtin(char **commands, t_env *env, t_token *tokens)
{
	t_token	*token;
	char	**new_commands;

	env = get_env(NULL);
	g_exit_status = 0;
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
	else
		execute_builtin_2(new_commands, env);
	if (ft_strcmp(commands[0], "exit") != 0)
		free_splits(new_commands);
}

void	execute_builtin_2(char **new_commands, t_env *env)
{
	if (ft_strcmp(new_commands[0], "cd") == 0)
		bi_cd(new_commands, env);
	else if (ft_strcmp(new_commands[0], "unset") == 0)
		bi_unset(new_commands, env);
	else if (ft_strcmp(new_commands[0], "export") == 0)
		bi_export(env, new_commands);
}
