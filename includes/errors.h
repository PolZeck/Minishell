/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:06:38 by lcosson           #+#    #+#             */
/*   Updated: 2025/03/27 12:45:21 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# define ERR_ARG				" ERROR: Invalid number of arguments !\n"
# define ERR_MALLOC				" ERROR: Memory allocation failed !\n"
# define ERR_PATH				" ERROR: Path not found !\n"
# define ERR_EXECVE				" ERROR: Execve failed !\n"
# define ERR_COMMAND			" ERROR: Command not found !\n"
# define ERR_ENV				" ERROR: Environment error !\n"
# define ERR_OPEN				" ERROR: Failed to open file !\n"
# define ERR_PIPE				" ERROR: Pipe failed !\n"
# define ERR_FORK				" ERROR: Fork failed !\n"
# define ERR_DUP2				" ERROR: Dup2 failed !\n"
# define ERR_SPLIT				" ERROR: Fonction split failed !\n"
# define ERR_MSG_NO_SUCH_FILE	" ERROR: No such file or directory\n"
# define ERR_MSG_NO_INFILE		" ERROR: No infile found !\n"
# define ERR_MSG_NO_OUTFILE		" ERROR: No outfile found !\n"
# define ERR_FD					" ERROR: Bad file descriptor\n"
# define ERR_PERMISSION			" ERROR: Permission denied\n"
# define ERR_ARG_HEREDOC		" ERROR: Invalid argument, heredoc needed !\n"

#endif