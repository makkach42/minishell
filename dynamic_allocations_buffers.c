/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_allocations_buffers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:09:53 by makkach           #+#    #+#             */
/*   Updated: 2025/04/30 11:36:16 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dyn_buf_init(t_dynbuf *buf, size_t initial_capacity)
{
    buf->data = malloc(initial_capacity);
    if (!buf->data)
        return (0);
    buf->size = initial_capacity;
    buf->pos = 0;
    return (1);
}

int	dyn_buf_ensure_capacity(t_dynbuf *buf, size_t additional_bytes)
{
    char	*new_data;
    size_t	new_size;

    if (buf->pos + additional_bytes >= buf->size)
    {
        new_size = buf->size * 2;
        if (buf->pos + additional_bytes >= new_size)
            new_size = buf->pos + additional_bytes + 1;
        new_data = malloc(new_size);
        if (!new_data)
            return (0);
        if (buf->pos > 0)
            memcpy(new_data, buf->data, buf->pos);
        free(buf->data);
        buf->data = new_data;
        buf->size = new_size;
    }
    return (1);
}

int	dyn_buf_add_char(t_dynbuf *buf, char c)
{
    if (!dyn_buf_ensure_capacity(buf, 1))
        return (0);
    buf->data[buf->pos++] = c;
    return (1);
}

int dyn_buf_add_str(t_dynbuf *buf, const char *str, size_t len)
{
    size_t	i;

    if (!dyn_buf_ensure_capacity(buf, len))
        return (0);
    i = 0;
    while (i < len)
    {
        buf->data[buf->pos + i] = str[i];
        i++;
    }
    buf->pos += len;
    return (1);
}

void	dyn_buf_finalize(t_dynbuf *buf)
{
    dyn_buf_ensure_capacity(buf, 1);
    buf->data[buf->pos] = '\0';
}

void	dyn_buf_free(t_dynbuf *buf)
{
    if (buf->data)
        free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->pos = 0;
}
