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

TAR = amirblum_final_project.tgz

# list of all the sources files
SRCS	=  	\
main.cpp \
BoundingBox.cpp \
BoundingBoxRenderer.cpp \
Camera.cpp \
CameraFollow.cpp \
CameraOpening.cpp \
CameraEnding.cpp \
CameraScripts.cpp \
City.cpp
CityBlock.cpp \
GameState.cpp \
HealthBar.cpp \
InputManager.cpp \
Mesh.cpp \
MeshRenderer.cpp \
Overlay.cpp \
PPBuffer.cpp \
RandUtils.cpp
RenderableSceneNode.cpp \
RenderComponent.cpp \
SceneNode.cpp \
ShaderIO.cpp \
SkyBox.cpp \
SoundManager.cpp \
Superhero.cpp \
TargetFollow.cpp \
World.cpp

HDRS	=  	\
BoundingBox.h \
BoundingBoxRenderer.h \
Camera.h \
CameraFollow.h \
CameraOpening.h \
CameraEnding.h \
CameraScripts.h \
City.h
CityBlock.h \
GameState.h \
HealthBar.h \
InputManager.h \
Mesh.h \
MeshRenderer.h \
Overlay.h \
PPBuffer.h \
RandUtils.h
RenderableSceneNode.h \
RenderComponent.h \
SceneNode.h \
Script.h \
ShaderIO.h \
SkyBox.h \
SoundManager.h \
Superhero.h \
TargetFollow.h \
World.h

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
APP = cgp_superhero

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
	tar czvf $(TAR) Makefile $(SRCS) $(HDRS) assets/ shaders/ include/ lib/ README 
	

.PHONY:
clean :
	rm -f $(OBJS) $(TAR) *~ $(APP)
