/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:02:19 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/16 15:02:22 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void	print_token(t_lst *tok_lst)
// {
// 	int	i;

// 	i = 1;
// 	while (tok_lst)
// 	{
// 		if (tok_lst->value != NULL)
// 			printf_fd(STDOUT_FILENO, "Token %d type : %d value : |%s|\n", i,
// tok_lst->type,
// 				tok_lst->value);
// 		if (tok_lst->expand == EXPAND && tok_lst->value != NULL)
// 			printf_fd(STDOUT_FILENO, "Token %d expand : EXPAND\n", i);
// 		else if (tok_lst->value != NULL)
// 			printf_fd(STDOUT_FILENO, "Token %d expand : NO_EXPAND\n", i);
// 		tok_lst = tok_lst->next;
// 		i++;
// 	}
// }