/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:14:45 by agaland           #+#    #+#             */
/*   Updated: 2024/10/30 21:21:25 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
/*
int	main(void)
{
	// int		nb = -2;
	t_list	*current;
	t_list	*root = ft_lstnew(NULL);
	current = root;

	while (current != NULL)
	{
		printf("%d\n", * (int *) current->content);
		current = current->next;
	}
}
*/
