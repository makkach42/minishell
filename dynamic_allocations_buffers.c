/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinamic_allocations_buffers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:09:53 by makkach           #+#    #+#             */
/*   Updated: 2025/04/30 11:10:05 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Initialize a dynamic buffer with initial capacity
int dyn_buf_init(t_dynbuf *buf, size_t initial_capacity)
{
    buf->data = malloc(initial_capacity);
    if (!buf->data)
        return (0);
    buf->size = initial_capacity;
    buf->pos = 0;
    return (1);
}

// Ensure buffer has enough space for additional bytes
int dyn_buf_ensure_capacity(t_dynbuf *buf, size_t additional_bytes)
{
    char *new_data;
    size_t new_size;

    if (buf->pos + additional_bytes >= buf->size)
    {
        // Double the size or increase by additional_bytes, whichever is larger
        new_size = buf->size * 2;
        if (buf->pos + additional_bytes >= new_size)
            new_size = buf->pos + additional_bytes + 1;
        
        new_data = malloc(new_size);
        if (!new_data)
            return (0);
        
        // Copy existing data to new buffer
        if (buf->pos > 0)
            memcpy(new_data, buf->data, buf->pos);
        
        free(buf->data);
        buf->data = new_data;
        buf->size = new_size;
    }
    return (1);
}

// Add a character to the buffer
int dyn_buf_add_char(t_dynbuf *buf, char c)
{
    if (!dyn_buf_ensure_capacity(buf, 1))
        return (0);
    
    buf->data[buf->pos++] = c;
    return (1);
}

// Add a string to the buffer
int dyn_buf_add_str(t_dynbuf *buf, const char *str, size_t len)
{
    if (!dyn_buf_ensure_capacity(buf, len))
        return (0);
    
    memcpy(buf->data + buf->pos, str, len);
    buf->pos += len;
    return (1);
}

// Finalize buffer (add null terminator)
void dyn_buf_finalize(t_dynbuf *buf)
{
    dyn_buf_ensure_capacity(buf, 1);
    buf->data[buf->pos] = '\0';
}

// Free buffer resources
void dyn_buf_free(t_dynbuf *buf)
{
    if (buf->data)
        free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->pos = 0;
}
