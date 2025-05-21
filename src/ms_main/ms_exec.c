/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:19:43 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/21 18:28:28 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_exec(char *input, t_env *env)
{
	char		**commands;
	t_token		*tokens;
	t_ast_node	*ast_root;
	t_shell		*shell;

	commands = tk_splitter(input, 0, 0);
	if (!ps_parsing(commands, 0))
	{
		ms_free(NULL, input, commands, NULL);
		return;
	}
	tokens = token_to_struct(commands);
	free_splits(commands);
	quote_fix(tokens);
	ast_root = build_ast(tokens);
	shell = get_shell();
	shell->tokens = tokens;
	shell->ast_root = ast_root;
	shell->env_list = env;
	if (collect_all_heredocs(ast_root) == -1)
		cleanup_heredocs(ast_root);
	execute_ast(ast_root, env, tokens, 0);
	cleanup_heredocs(ast_root);
	cleanup_shell(shell, 0);
	return;
}
