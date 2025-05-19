/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:43:29 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/17 19:19:05 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void handle_heredoc_input(const char *delimiter, int fd)
{
    char *line;

    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
}

int execute_heredoc(t_ast_node *node)
{
    int fd;

    if (!node || node->type != TOKEN_HEREDOC)
        return (-1);
    fd = open(node->heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0)
        return (-1);
    handle_heredoc_input(node->args[0], fd);
    close(fd);
    return (0);
}

void collect_all_heredocs(t_ast_node *node)
{
    static int heredoc_count = 0;
    char filename[64];

    if (!node)
        return;
    if (node->type == TOKEN_HEREDOC)
    {
        snprintf(filename, sizeof(filename), ".heredoc_%d_%d", getpid(), heredoc_count++);
        if (node->heredoc_file)
            free(node->heredoc_file);
        node->heredoc_file = strdup(filename);
        execute_heredoc(node);
    }
    collect_all_heredocs(node->left);
    collect_all_heredocs(node->right);
}