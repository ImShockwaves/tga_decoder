/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccharrie <ccharrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 15:08:29 by ccharrie          #+#    #+#             */
/*   Updated: 2019/05/15 20:43:15 by ccharrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TGA_H
# define TGA_H

#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t		t_ubyte;
typedef uint32_t	t_uint;

typedef struct	s_texture
{
	int			width;
	int			height;
	int			format;
	int			internalFormat;
	t_uint		id;
	t_ubyte		*texels;
}				t_texture;

#pragma pack(push, 1)
typedef struct s_header
{
	t_ubyte		id_length;
	t_ubyte		colormap_type;        
	t_ubyte		image_type;      
	short		cm_first_entry;      
	short		cm_length;          
	t_ubyte		cm_size;          
	short		x_origin;          
	short		y_origin;            
	short		width;               
	short		height;              
	t_ubyte		pixel_depth;       
	t_ubyte		image_descriptor;  
}				t_header;
#pragma pack(pop)

t_uint texId = 0;

#endif