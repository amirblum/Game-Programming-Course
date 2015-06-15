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

TAR = amirblum_ex4.tgz

# list of all the sources files
SRCS	=  	main.cpp		\
		World.cpp		\
        GameState.cpp \
        Camera.cpp  \
		CameraFollow.cpp \
        CameraOpening.cpp \
        CameraEnding.cpp \
        CameraScripts.cpp \
        InputManager.cpp    \
        SoundManager.cpp \
		ParticleSystem.cpp \
		AsteroidParticleSystem.cpp \
        AsteroidRigidBody.cpp \
        ExplosionParticleSystem.cpp \
        ThrusterParticleSystem.cpp \
		Mesh.cpp \
		SceneNode.cpp \
		RenderableSceneNode.cpp \
		Ship.cpp 	\
		HealthBar.cpp \
        Shield.cpp \
        BlackHole.cpp \
        Beacon.cpp \
		DummyObject.cpp \
		SkyBox.cpp \
        RenderComponent.cpp  \
        PhysicsSimulation.cpp \
        RigidBody.cpp \
		PhysicsComponent.cpp \
        Overlay.cpp \
		ShaderIO.cpp \
        RandUtils.cpp

HDRS	=  	\
		World.h		\
        GameState.h \
        Camera.h  \
		CameraFollow.h \
        CameraOpening.h \
        CameraEnding.h \
        CameraScripts.h \
        InputManager.h    \
        SoundManager.h \
		ParticleSystem.h \
        AsteroidParticleSystem.h \
        AsteroidRigidBody.h \
        ExplosionParticleSystem.h \
        ThrusterParticleSystem.h \
		Mesh.h \
		SceneNode.h \
		RenderableSceneNode.h \
		Ship.h 	\
		HealthBar.h \
        Shield.h \
        BlackHole.h \
        Beacon.h \
		DummyObject.h \
		SkyBox.h \
        RenderComponent.h  \
        PhysicsSimulation.h \
        RigidBody.h \
		PhysicsComponent.h \
        Overlay.h \
		ShaderIO.h	\
		RandUtils.h

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
APP = cgp_ex4

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
