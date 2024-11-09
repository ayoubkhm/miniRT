#include "../includes/minirt.h"

t_list  *ft_lstnew(void *content)
{
    t_list  *new_node;

    new_node = malloc(sizeof(t_list));
    if (!new_node)
        return (NULL);
    new_node->content = content;
    new_node->next = NULL;
    return (new_node);
}

void    ft_lstadd_front(t_list **lst, t_list *new_node)
{
    if (!lst || !new_node)
        return;
    new_node->next = *lst;
    *lst = new_node;
}