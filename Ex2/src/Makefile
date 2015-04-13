#################################################################
#
#				Warning!!!!
#				============
#	
# this make file compiles all the sources with implicit rules
# therefore in order to make sure all the changes were made 
# run it with "make clean all"
#
##################################################################

TAR = amirblum_cgp_ex2.tgz

# list of all the sources files
SRCS	=  	main.cpp		\
		World.cpp		\
		Corridor.cpp 	\
		ShaderIO.cpp	\
        Camera.cpp  \
        InputManager.cpp    \
        Renderable.cpp  \
        Controller.cpp	\
		PPBuffer.cpp	\
		bimage.cpp \

HDRS	=  	World.h	\
			Corridor.h 	\
            ShaderIO.h	\
            Camera.cpp  \
            InputManager.h    \
            Renderable.h  \
            Controller.h	\
			PPBuffer.h	\
			bimage.h

# compiler
CC	= g++

INCLUDE = -I/usr/include -I/usr/X11R6/include -I/cs/course/current/cg/include

OBJS = $(subst .cpp,.o,$(SRCS)) 		#substituting .cc -> .o

# in order to run in debug mode use the command "make DBG=1"
ifdef DBG

	CFLAGS = 	\
		-Wall 	\
		-Werror \
		-DDEBUG	\
		-O0		\
		-g3		\
		-Wno-deprecated
					
						
else

	CFLAGS = 		\
		-Wall		\
		-DNDEBUG    \
		-O2
				 
endif


# linker
LD	= $(CC)
LDFLAGS	=


# the name of the lib to create 
APP = cgp_ex2

# each lib should add with -l flag
LIBS =  -L/usr/lib -lm -lGLEW -lglut -lGL -lGLU -ldl -L/usr/X11R6/lib


# rule to change the suffix of the sources files
OBJS	= ${SRCS:.cpp=.o}

# clear out all suffixes
.SUFFIXES:
# list only those we use
.SUFFIXES: .o .cpp

# define a suffix rule for .c -> .o
.cpp.o :
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

##################################
# 	default target 		##
##################################
all : $(APP)

$(APP) : $(OBJS)
	$(LD)  $(OBJS) $(LIBS) -o $(APP)

$(OBJS) : $(SRCS)


tar:
	tar czvf $(TAR) Makefile $(SRCS) $(HDRS) README
	

.PHONY:
clean :
	rm -f $(OBJS) $(TAR) *~ $(APP)
