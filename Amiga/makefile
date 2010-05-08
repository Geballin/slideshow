# makefile pour Slideshow
# 2010/01/11  G.Ballin

CC = gcc
CFlags = -c -o
# LFlags = -noixemul -lamiga -lsdl_libnix -lSDL_image -lgl_dummy -lgcc -lm -o
LFlags = -lamiga -lsdl -lSDL_image -lgl_dummy -lgcc -lm -o

slideshow : ../file_list.o ../fontdata.o ../main.o ../SDLFont.o ../slideshow.o
	$(CC) $^ $(LFlags) $@

%.o : %.c
	$(CC) $< $(CFlags) $@

file_list.o : ../file_list.cpp ../file_list.h
fontdata.o : ../fontdata.cpp ../fontadata.h
main.o : ../main.cpp ../file_list.h ../slideshow.h ../cache.h
SDLFont.o : ../SDLFont.cpp ../SDLFont.h
slideshow.o : ../slideshow.cpp ../slideshow.h ../cache.h ../SDLFont.h ../fontdata.h ../file_lst.h

clean :
	delete :/\#\?.o
