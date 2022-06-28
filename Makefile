#######################################################################
##                  Marcel le Raytracer Parallele                    ##
#######################################################################
##         Eric Mugnier - Pierre Lena - Guillaume Bourelly           ##
##                     Jean Francois Morcillo                        ##
####################################################################### 
## First Mdofication: 01/05/1999                                     ##
#######################################################################
## Completely modified by Pierre Lena on January 2002                ##
#######################################################################

CPP          = g++
DEL          = rm -f
AR           = ar
RANLIB       = ranlib
STRIP        = strip
ECHO         = echo

DOC_DIR            = docs
RENDERER_DIR       = Renderer
TOOLS_DIR          = tools

CORE_SRC_DIR       = $(RENDERER_DIR)/Core
UTILS_SRC_DIR      = $(RENDERER_DIR)/Utils
NETWORK_SRC_DIR	   = $(RENDERER_DIR)/Utils/Network
CORE_OBJ_DIR       = $(CORE_SRC_DIR)/obj
UTILS_OBJ_DIR      = $(UTILS_SRC_DIR)/obj
NETWORK_OBJ_DIR    = $(NETWORK_SRC_DIR)/obj
INTERFACE_OBJ_DIR  = $(INTERFACE_SRC_DIR)/obj

#IO Formats source
3DS_SRC_DIR        = $(RENDERER_DIR)/Utils/IOFormat/3ds
OBJ_SRC_DIR        = $(RENDERER_DIR)/Utils/IOFormat/obj
TGA_SRC_DIR        = $(RENDERER_DIR)/Utils/IOFormat/tga
FLIC_SRC_DIR       = $(RENDERER_DIR)/Utils/IOFormat/flic
PNG_SRC_DIR        = $(RENDERER_DIR)/Utils/IOFormat/png
BMP_SRC_DIR        = $(RENDERER_DIR)/Utils/IOFormat/bmp
JPG_SRC_DIR        = $(RENDERER_DIR)/Utils/IOFormat/jpg

#IO Formats objects
3DS_OBJ_DIR        = $(3DS_SRC_DIR)/obj
OBJ_OBJ_DIR        = $(OBJ_SRC_DIR)/obj
TGA_OBJ_DIR        = $(TGA_SRC_DIR)/obj
FLIC_OBJ_DIR       = $(FLIC_SRC_DIR)/obj
PNG_OBJ_DIR        = $(PNG_SRC_DIR)/obj


INCLUDE_DIR        = -I$(RENDERER_DIR) -I$(3DS_SRC_DIR) -I$(OBJ_SRC_DIR) -I$(TGA_SRC_DIR) -I$(FLIC_SRC_DIR)

BIN_DIR = bin

TGZ_FILENAME = 'Marcel-'`date +"%Y-%m-%d"`'.tgz'

#PROFILING=-fprofile-arcs -ftest-coverage
CFLAGS=-std=gnu++11 -D_DEBUG_ -fpermissive -Wwrite-strings -Wno-deprecated -Wunused -g

LIBFLAGS	= -lpthread -ljpeg -lz

CORE_OBJ=	$(CORE_OBJ_DIR)/Tuple.o 			\
		$(CORE_OBJ_DIR)/Color.o 				\
		$(CORE_OBJ_DIR)/RawVec3.o 				\
		$(CORE_OBJ_DIR)/Ray.o 				\
		$(CORE_OBJ_DIR)/Vector.o 				\
		$(CORE_OBJ_DIR)/Point.o 				\
		$(CORE_OBJ_DIR)/Omni.o	 				\
		$(CORE_OBJ_DIR)/Light.o					\
		$(CORE_OBJ_DIR)/Spot.o					\
		$(CORE_OBJ_DIR)/Object.o 				\
		$(CORE_OBJ_DIR)/Plan.o 					\
		$(CORE_OBJ_DIR)/Droite.o 				\
		$(CORE_OBJ_DIR)/Raytrace.o				\
		$(CORE_OBJ_DIR)/Texture.o				\
		$(CORE_OBJ_DIR)/ColorTexture.o			\
		$(CORE_OBJ_DIR)/GrayTexture.o   		\
		$(CORE_OBJ_DIR)/Triangle.o				\
		$(CORE_OBJ_DIR)/Polygon.o				\
		$(CORE_OBJ_DIR)/Matrix.o				\
		$(CORE_OBJ_DIR)/Animation.o				\
		$(CORE_OBJ_DIR)/KeyFrame.o				\
		$(CORE_OBJ_DIR)/nTree.o					\
		$(CORE_OBJ_DIR)/Octree.o				\
		$(CORE_OBJ_DIR)/QuadTree.o				\
		$(CORE_OBJ_DIR)/Material.o				\
		$(CORE_OBJ_DIR)/CullingBox.o			\
		$(CORE_OBJ_DIR)/Camera.o				\
		$(CORE_OBJ_DIR)/Scene.o					\
		$(CORE_OBJ_DIR)/Image.o					\
		$(CORE_OBJ_DIR)/RowDispatcher.o			\
		$(CORE_OBJ_DIR)/AxisAlignedBox.o

UTILS_OBJ= $(UTILS_OBJ_DIR)/Utils.o				\
		$(UTILS_OBJ_DIR)/ImageUtils.o			\
		$(UTILS_OBJ_DIR)/MessageHandler.o

NETWORK_OBJ= $(NETWORK_OBJ_DIR)/Socket.o 		\
			 $(NETWORK_OBJ_DIR)/UdpSocket.o 	\
			 $(NETWORK_OBJ_DIR)/MessageSocket.o

TOOLS_OBJ=  $(ASCLOADER_OBJ) 			\
			$(3DSLOADER_OBJ)

TEST_OBJ=	$(CORE_OBJ_DIR)/MakeScene.o

SERVER_OBJ= $(CORE_OBJ_DIR)/Color.o 				\
			$(CORE_OBJ_DIR)/Image.o 				\
			$(CORE_OBJ_DIR)/RowDispatcher.o         \
		    $(NETWORK_OBJ_DIR)/ServerSocket.o

CLIENT_OBJ= $(NETWORK_OBJ_DIR)/ClientSocket.o 	    \
			$(CORE_OBJ_DIR)/RowDispatcherClient.o   \
			$(CORE_OBJ_DIR)/SceneClient.o 			\
		    $(CORE_OBJ_DIR)/ImageClient.o

TGA_OBJ= $(TGA_OBJ_DIR)/tga.o 						\
		 $(TGA_OBJ_DIR)/tgaread.o 					\
		 $(TGA_OBJ_DIR)/tgawrite.o

FLIC_OBJ= $(FLIC_OBJ_DIR)/flic.o
PNG_OBJ = $(PNG_OBJ_DIR)/lodepng.o
3DS_OBJ = $(3DS_OBJ_DIR)/l3ds.o
OBJ_OBJ = $(OBJ_OBJ_DIR)/tiny_obj_loader.o

I_OBJ   = $(3DS_OBJ)	\
		  $(OBJ_OBJ)
O_OBJ   = $(TGA_OBJ)	\
		  $(FLIC_OBJ)	\
		  $(PNG_OBJ)
IO_OBJ  = $(I_OBJ) $(O_OBJ)

LIB      =

PROGRAM  = 	Marcel

all: Marcel Server Client

##################################################################################
$(CORE_OBJ_DIR)/%.o: $(CORE_SRC_DIR)/%.cc $(CORE_SRC_DIR)/%.hh
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

$(UTILS_OBJ_DIR)/%.o: $(UTILS_SRC_DIR)/%.cc $(UTILS_SRC_DIR)/%.hh
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

$(TINYTHREAD_OBJ_DIR)/%.o: $(TINYTHREAD_SRC_DIR)/%.cpp $(TINYTHREAD_SRC_DIR)/%.h
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

$(NETWORK_OBJ_DIR)/%.o: $(NETWORK_SRC_DIR)/%.cc $(NETWORK_SRC_DIR)/%.hh Renderer/Utils/Network/define.hh
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

$(TGA_OBJ_DIR)/%.o: $(TGA_SRC_DIR)/%.c
	$(CPP) -g -Wwrite-strings -I$(TGA_SRC_DIR) -c $< -o $@
$(FLIC_OBJ_DIR)/%.o: $(FLIC_SRC_DIR)/%.cc
	$(CPP) -g -std=c++11 -c -I$(FLIC_SRC_DIR)/.. $< -o $@
$(PNG_OBJ_DIR)/%.o: $(PNG_SRC_DIR)/%.cpp
	$(CPP) -g -std=c++11 -c -I$(PNG_SRC_DIR)/.. $< -o $@
$(3DS_OBJ_DIR)/%.o: $(3DS_SRC_DIR)/%.cc
	$(CPP) -g -std=c++11 -c -I$(3DS_SRC_DIR)/.. $< -o $@
$(OBJ_OBJ_DIR)/%.o: $(OBJ_SRC_DIR)/%.cc
	$(CPP) -g -std=c++11 -c -I$(OBJ_SRC_DIR)/.. $< -o $@

Renderer/server.o: Renderer/server.cc
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

Renderer/client.o: Renderer/client.cc
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

Renderer/Marcel.o: Renderer/Marcel.cc
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

Renderer/test.o: Renderer/test.cc
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

test_udp_server.o: test_udp_server.cc
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

test_udp_client.o: test_udp_client.cc
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

glm.o: glm.cpp
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

##################################################################################
Server: $(SERVER_OBJ) $(UTILS_OBJ) $(NETWORK_OBJ) $(IO_OBJ) Makefile Renderer/server.o
	$(CPP) $(CFLAGS) $(SERVER_OBJ) $(UTILS_OBJ) $(NETWORK_OBJ) $(IO_OBJ) Renderer/server.o -o $@ $(LIBFLAGS)

Client:  $(CLIENT_OBJ) $(CORE_OBJ) $(UTILS_OBJ) $(NETWORK_OBJ) $(IO_OBJ)  Makefile Renderer/client.o
	$(CPP) $(CFLAGS) $(CORE_OBJ) $(CLIENT_OBJ) $(UTILS_OBJ) $(NETWORK_OBJ) $(IO_OBJ) Renderer/client.o -o $@ $(LIBFLAGS)

Marcel: $(CORE_OBJ) $(UTILS_OBJ) $(IO_OBJ) Makefile Renderer/Marcel.o
	$(CPP) $(CFLAGS) $(CORE_OBJ) $(UTILS_OBJ) $(IO_OBJ) Renderer/Marcel.o -o $@ $(LIBFLAGS)

MyGlm: Renderer/Core/obj/Vector.o Renderer/Core/obj/RawVec3.o glm.o
	$(CPP) $(CFLAGS) Renderer/Core/obj/Vector.o  Renderer/Core/obj/RawVec3.o glm.o -o $@ $(LIBFLAGS)

MakeTestScene: $(TEST_OBJ) Makefile
	$(CPP) $(TEST_OBJ) -o $@

test: Renderer/Core/obj/Color.o Renderer/Core/obj/GenericObject.o Renderer/Utils/obj/ImageUtils.o Renderer/test.o $(IO_OBJ)
	$(CPP) -g Renderer/Core/obj/Color.o Renderer/Core/obj/GenericObject.o Renderer/Utils/obj/ImageUtils.o $(IO_OBJ) Renderer/test.o -o $@ -ljpeg

test_udp_server: test_udp_server.o $(NETWORK_OBJ_DIR)/Socket.o $(NETWORK_OBJ_DIR)/UdpSocket.o
	$(CPP) $(CFLAGS) $(NETWORK_OBJ_DIR)/Socket.o $(NETWORK_OBJ_DIR)/UdpSocket.o test_udp_server.o -o $@

test_udp_client: test_udp_client.o $(NETWORK_OBJ_DIR)/Socket.o $(NETWORK_OBJ_DIR)/UdpSocket.o
	$(CPP) $(CFLAGS) $(NETWORK_OBJ_DIR)/Socket.o $(NETWORK_OBJ_DIR)/UdpSocket.o test_udp_client.o -o $@

##################################################################################
clean_marcel:
	@$(DEL) -f core $(RENDERER_OBJ) $(CORE_SRC_SIR)/*.tab.* $(CORE_SRC_SIR)/*.output $(CORE_SRC_SIR)/pov_lex.c *~ $(DOC_DIR)/*.html $(DOC_DIR)/*.gif $(DOC_DIR)/*.db $(DOCIFIED_DIR)/*

clean_obj:
	@echo "Cleaning obj files..."
	@$(DEL) -f $(RENDERER_DIR)/server.o
	@$(DEL) -f $(RENDERER_DIR)/client.o
	@$(DEL) -f $(RENDERER_DIR)/Marcel.o
	@$(DEL) -f *.stackdump core $(CORE_OBJ) $(SERVER_OBJ) $(CLIENT_OBJ) $(NET_OBJ) $(UTILS_OBJ) $(INTERFACE_OBJ) $(TINYTHREAD_OBJ) $(NETWORK_OBJ) $(TGA_OBJ) $(FLIC_OBJ)
	@$(DEL) -f $(CORE_OBJ_DIR)/*.gcno $(NET_OBJ_DIR)/*.gcno  $(INTERFACE_OBJ_DIR)/*.gcno $(TINYTHEAD_OBJ)/*.gcno
	@$(DEL) -f $(CORE_OBJ_DIR)/*.gcda $(NET_OBJ_DIR)/*.gcda  $(INTERFACE_OBJ_DIR)/*.gcda $(TINYTHEAD_OBJ)/*.gcda
	@echo "Done."

clean_src:
	@echo "Cleaning sources files..."
	@$(DEL) $(TOOLS_SRC_DIR)/*~ $(CORE_SRC_DIR)/*~ $(NET_SRC_DIR)/*~ 
	@echo "Done."

clean: clean_obj clean_src

##################################################################################
dist:
	tar -zcvf  $(TGZ_FILENAME) --exclude=*.o --exclude=*.tab.* --exclude=pov_lex.c --exclude=asc.lex.c --exclude=*.output --exclude=*~ *
##################################################################################

