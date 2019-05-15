/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccharrie <ccharrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 20:51:33 by ccharrie          #+#    #+#             */
/*   Updated: 2019/05/15 20:51:34 by ccharrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tga.h"

static void		get_texture_info(t_header *header, t_texture *texinfo)
{
	texinfo->width = header->width;
	texinfo->height = header->height;
	texinfo->format = GL_RGBA;
	texinfo->internalFormat = 4;
}

static void		read_tga_color(FILE *fp, t_texture *texinfo)
{
	int i;

	i = -1;
	while (++i < texinfo->width * texinfo->height)
	{
		texinfo->texels[(i * 4) + 2] = (t_ubyte)fgetc(fp);
		texinfo->texels[(i * 4) + 1] = (t_ubyte)fgetc(fp);
		texinfo->texels[(i * 4) + 0] = (t_ubyte)fgetc(fp);
		texinfo->texels[(i * 4) + 3] = (t_ubyte)fgetc(fp);
	}
}

static t_texture	*read_tga_file(const char *filename)
{
	FILE		*fp;
	t_texture	*texinfo;
	t_header	header;
	int			texels_size;

	fp = fopen(filename, "rb");
	if (!fp)
	{
		printf("error: couldn't open \"%s\"!\n", filename);
		return (NULL);
	}
	fread(&header, sizeof (t_header), 1, fp);
	if (!(texinfo = (t_texture *)malloc(sizeof(t_texture))))
		return (NULL);
	get_texture_info(&header, texinfo);
	texels_size = texinfo->width * texinfo->height * texinfo->internalFormat;
	if (!(texinfo->texels = (t_ubyte *)malloc (sizeof (t_ubyte) * texels_size)))
		return (NULL);
	read_tga_color(fp, texinfo);
	fclose(fp);
	return (texinfo);
}

t_uint				load_tga_texture(const char *filename)
{
	t_texture	*tga_tex;
	int			alignment;
	int			id;
	
	id = 0;
	tga_tex = read_tga_file(filename);
	if (tga_tex && tga_tex->texels)
	{
		glGenTextures(1, &tga_tex->id);
		glBindTexture(GL_TEXTURE_2D, tga_tex->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		gluBuild2DMipmaps(GL_TEXTURE_2D, tga_tex->internalFormat,
				tga_tex->width, tga_tex->height,
				tga_tex->format, GL_UNSIGNED_BYTE, tga_tex->texels);
		glPixelStorei (GL_UNPACK_ALIGNMENT, alignment);
		id = tga_tex->id;
		free(tga_tex->texels);
		free(tga_tex);
	}
	return (id);
}

static void		cleanup()
{
	glDeleteTextures(1, &texId);
}

static void			init (const char *filename)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	texId = load_tga_texture(filename);
	if (!texId)
		exit(EXIT_FAILURE);
}

static void			reshape(int w, int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, (int)w, (int)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, w/(GLdouble)h, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

static void			display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTranslatef(0.0, 0.0, -5.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

static void			keyboard(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;
	if (key == 27)
		exit (0);
}

int					main (int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: %s <filename.tga>\n", argv[0]);
		return -1;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow(argv[1]);
	atexit(cleanup);
	init(argv[1]);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return (0);
}
