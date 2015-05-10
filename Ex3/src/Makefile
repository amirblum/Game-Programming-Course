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

TAR = amirblum_cgp_ex3.tgz

# list of all the sources files
SRCS	=  	main.cpp		\
		World.cpp		\
        Camera.cpp  \
		CameraFollow.cpp \
        InputManager.cpp    \
		ParticleSystem.cpp \
		AsteroidParticleSystem.cpp \
		Mesh.cpp \
		SceneNode.cpp \
		RenderableSceneNode.cpp \
		Ship.cpp 	\
		HealthBar.cpp \
		DummyObject.cpp \
		SkyBox.cpp \
        RenderComponent.cpp  \
		PhysicsComponent.cpp \
		ShaderIO.cpp	

HDRS	=  	\
		World.h		\
        Camera.h  \
		CameraFollow.h \
        InputManager.h    \
		ParticleSystem.h \
		AsteroidParticleSystem.h \
		Mesh.h \
		SceneNode.h \
		RenderableSceneNode.h \
		Ship.h 	\
		HealthBar.h \
		DummyObject.h \
		SkyBox.h \
        RenderComponent.h  \
		PhysicsComponent.h \
		ShaderIO.h	\
		Utils.h

# compiler
CC	= g++

# in order to run in debug mode use the command "make DBG=1"
ifdef DBG

	CFLAGS = 	\
		-Wall 	\
		-Werror \
		-DDEBUG	\
		-O0		\
		-g3		\
		-Wno-deprecated \
		-std=c++11	
					
						
else

	CFLAGS = 		\
		-Wall		\
		-DNDEBUG    \
		-O2			\
		-std=c++11
				 
endif


# linker
LD	= $(CC)
LDFLAGS	=

# Include Headers
INCLUDE = -I/usr/include -I./include

# each lib should add with -l flag
LIBS =  -L/usr/lib -L./lib -lm -lGLEW -lglut -lGL -lGLU -lopenal -l:libalut.so.0 -ldl -lsoil2 -lassimp

# the name of the lib to create 
APP = cgp_ex3

# rule to change the suffix of the sources files
OBJS	= ${SRCS:.cpp=.o}

# define a suffix rule for .c -> .o
.cpp.o :
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

##################################
# 	default target 		##
##################################
all : $(APP)

$(APP) : $(OBJS)
	$(LD)  $(OBJS) $(LIBS) -o $(APP) $(LDFLAGS)

$(OBJS) : $(SRCS)


tar:
	tar czvf $(TAR) Makefile $(SRCS) $(HDRS) assets/ shaders/ include/ README
	

.PHONY:
clean :
	rm -f $(OBJS) $(TAR) *~ $(APP)
