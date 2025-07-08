/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haatwata <haatwata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 03:02:27 by haatwata          #+#    #+#             */
/*   Updated: 2025/02/09 16:24:41 by haatwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft/libft.h"

int		ft_printf(const char *format, ...);
void	put_space(int width);
void	put_ptr_as_hex(uintptr_t ptr_uint);
int		base_n_len(unsigned int value, unsigned int n);
void	put_as_base_n(unsigned int value, unsigned int n, bool is_big);
int		put_type_x(va_list *ap, int width, bool is_big);
int		put_type_u(va_list *ap, int width);
int		put_type_d(va_list *ap, int width);
int		put_type_p(va_list *ap, int width);
int		put_type_s(va_list *ap, int width);
int		put_type_c(va_list *ap, int width);

#endif
