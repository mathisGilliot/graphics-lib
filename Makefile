# Variable definitions.

PLATFORM	= $(shell uname)
CC		= gcc
OBJDIR		= ./objs
TESTS		= ./tests
INCLUDES	= ./include
INCFLAGS	:= -I${INCLUDES}
SRC		= ./src
#OPTFLAGS	:= -Os -DNDEBUG
OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99

# The list of objects to include in the library

LIBEIOBJS	:=  ${OBJDIR}/ei_application.o \
		${OBJDIR}/ei_widgetframe.o \
		${OBJDIR}/ei_widgetbutton.o \
		${OBJDIR}/ei_widget.o \
		${OBJDIR}/ei_widgetclass.o \
		${OBJDIR}/ei_geometrymanager.o \
		${OBJDIR}/ei_geometry_placer.o \
		${OBJDIR}/ei_event_manager.o \
		${OBJDIR}/ei_event.o \
		${OBJDIR}/ei_draw_utilities.o \
		${OBJDIR}/ei_widgettoplevel.o



# Platform specific definitions (OS X, Linux, Windows)

ifeq (${PLATFORM},Darwin)

# Building for Mac OS X	
	PLATDIR		= _osx
	INCFLAGS	:= ${INCFLAGS} -I/opt/local/include -I/opt/local/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
	LIBS		= ${LIBEIBASE} -L/opt/local/lib -lfreeimage -lSDL -lSDL_ttf -lSDL_gfx -framework AppKit

else
	ifeq (${PLATFORM},Linux)

# Building for Linux	
	PLATDIR		= _x11
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
#	ARCH	        = 32
	ARCH	        = 64
	LIBEIBASE	= ${PLATDIR}/libeibase${ARCH}.a
	LIBS		= ${LIBEIBASE} -L${PLATDIR} -lSDL -lSDL_ttf -lSDL_gfx${ARCH} -lfreeimage${ARCH} -lm -lstdc++

	else

# Building for Windows
	PLATDIR		= _win
	INCFLAGS	:= ${INCFLAGS} -I/usr/include -I/usr/include/SDL -I/usr/local/include/SDL
	LINK		= ${CC} -mwindows -mno-cygwin
	LIBEI		= ${OBJDIR}/libei.lib
	LIBEIBASE	= ${PLATDIR}/libeibase.lib
	LIBS		= -lmingw32 ${LIBEIBASE} /usr/lib/SDL_ttf.lib /usr/local/lib/libSDL_gfx.a -L/usr/lib -lSDL -lFreeImage

	endif
endif



# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
			minimal frame button hello_world flag frame_text\
			puzzle minesweeper relative_parameters\
			hello_world_supp button_img frame_img 

all : ${TARGETS}


########## Compilation en fichiers objets

${OBJDIR}/%.o : ${SRC}/%.c
	${CC} ${CCFLAGS} ${INCFLAGS} $< -o $@

########## Test-programs

# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE}
	${LINK} -o minimal ${OBJDIR}/minimal.o ${LIBS}

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o

# frame

${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

frame : ${OBJDIR}/frame.o  ${LIBEIBASE} ${LIBEI} 
	${LINK} -o frame ${OBJDIR}/frame.o ${LIBEI} ${LIBS}


# frame_text

${OBJDIR}/frame_text.o : ${TESTS}/frame_text.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame_text.c -o ${OBJDIR}/frame_text.o

frame_text : ${OBJDIR}/frame_text.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame_text ${OBJDIR}/frame_text.o ${LIBEI} ${LIBS}

# frame_img

${OBJDIR}/frame_img.o : ${TESTS}/frame_img.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame_img.c -o ${OBJDIR}/frame_img.o

frame_img : ${OBJDIR}/frame_img.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame_img ${OBJDIR}/frame_img.o ${LIBEI} ${LIBS}


# flag

${OBJDIR}/flag.o : ${TESTS}/flag.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/flag.c -o ${OBJDIR}/flag.o

flag : ${OBJDIR}/flag.o  ${LIBEIBASE} ${LIBEI} 
	${LINK} -o flag ${OBJDIR}/flag.o ${LIBEI} ${LIBS} 


# relativ_parameters

${OBJDIR}/relative_parameters.o : ${TESTS}/relative_parameters.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/relative_parameters.c -o ${OBJDIR}/relative_parameters.o

relative_parameters : ${OBJDIR}/relative_parameters.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o relative_parameters ${OBJDIR}/relative_parameters.o ${LIBEI} ${LIBS}


# button

button : ${OBJDIR}/button.o ${LIBEIBASE} ${LIBEI} 
	${LINK} -o button ${OBJDIR}/button.o ${LIBEI} ${LIBS}


${OBJDIR}/button.o : ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o

# button_img

button_img : ${OBJDIR}/button_img.o ${LIBEIBASE} ${LIBEI} 
	${LINK} -o button_img ${OBJDIR}/button_img.o ${LIBEI} ${LIBS}


${OBJDIR}/button_img.o : ${TESTS}/button_img.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button_img.c -o ${OBJDIR}/button_img.o

# hello_world

hello_world : ${OBJDIR}/hello_world.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world ${OBJDIR}/hello_world.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world.o : ${TESTS}/hello_world.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world.c -o ${OBJDIR}/hello_world.o

# toplevel

toplevel : ${OBJDIR}/toplevel.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o toplevel ${OBJDIR}/toplevel.o ${LIBEI} ${LIBS}

${OBJDIR}/toplevel.o : ${TESTS}/toplevel.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/toplevel.c -o ${OBJDIR}/toplevel.o

# hello_world_supp

hello_world_supp : ${OBJDIR}/hello_world_supp.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world_supp ${OBJDIR}/hello_world_supp.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world_supp.o : ${TESTS}/hello_world_supp.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world_supp.c -o ${OBJDIR}/hello_world_supp.o


# puzzle

puzzle : ${OBJDIR}/puzzle.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${OBJDIR}/puzzle.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o


# minesweeper

minesweeper : ${OBJDIR}/minesweeper.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o minesweeper ${OBJDIR}/minesweeper.o ${LIBEI} ${LIBS}

${OBJDIR}/minesweeper.o : ${TESTS}/minesweeper.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minesweeper.c -o ${OBJDIR}/minesweeper.o



# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	ar rcs ${LIBEI} ${LIBEIOBJS}



# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg



# Removing all built files, and temporary files.

clean:
	rm -f ${TARGETS}
	rm -f *.exe
	rm -f ${OBJDIR}/*
	rm -f *~
	rm -f ${SRC}/*~
	rm -f ${INCLUDES}/*~
	rm -f ${TESTS}/*~