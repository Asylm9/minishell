/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:21:57 by agaland           #+#    #+#             */
/*   Updated: 2024/11/02 23:55:51 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*newnode;
	void	*temp;

	if (!lst || !f || !del)
		return (NULL);
	newlst = NULL;
	newnode = NULL;
	while (lst)
	{
		temp = f(lst->content);
		newnode = ft_lstnew(temp);
		if (!newnode)
		{
			del(temp);
			ft_lstclear(&newlst, (*del));
			return (NULL);
		}
		ft_lstadd_back(&newlst, newnode);
		lst = lst->next;
	}
	ft_lstclear(&lst, (*del));
	return (newlst);
}
