/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   at_build_ast_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 02:58:46 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 02:59:08 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*build_ast(t_token *tokens)
{
	t_token		*token;
	t_ast_node	*left;
	t_ast_node	*pipe;

	token = tokens;
	left = parse_command(&token);
	if (token && token->type == TOKEN_PIPE)
	{
		token = token->next;
		pipe = create_node(TOKEN_PIPE);
		pipe->left = left;
		pipe->right = build_ast(token);
		return (pipe);
	}
	return (left);
}

void	hd_flag_definer(t_ast_node *node, t_token *token)
{
	if (token->eof_envvar)
		node->eof_envvar = true;
	if (token->eof_inquote)
		node->eof_inquote = true;
	else
	{
		node->eof_envvar = false;
		node->eof_inquote = false;
	}
}
