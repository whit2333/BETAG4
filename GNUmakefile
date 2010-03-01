# --------------------------------------------------------------
# GNUmakefile beta simulation        Whitney Armstrong 12/16/07
# --------------------------------------------------------------

name := BETA
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

#   CPPFLAGS += `aida-config --include `
#   LDLIBS += `aida-config --lib `
# ROOT
   CPPFLAGS += -I$(ROOTSYS)/include
   LDLIBS += $(shell root-config --glibs) -lGeomPainter -lGeom
 
# ROOT and  GNU Scientific Librarires and 
   CPPFLAGS += -g -I$(shell root-config --incdir )  
   LDLIBS += -lgsl -lgslcblas -lm  $(shell root-config --nonew --glibs)
# VGM
   CPPFLAGS  += -I$(VGM_INSTALL)/packages/VGM/include
   CPPFLAGS  += -I$(VGM_INSTALL)/packages/BaseVGM/include
   CPPFLAGS  += -I$(VGM_INSTALL)/packages/Geant4GM/include
   CPPFLAGS  += -I$(VGM_INSTALL)/packages/RootGM/include
   LDLIBS += -L$(VGM_INSTALL)/lib/$(VGM_SYSTEM)
   LDLIBS += -lGeant4GM -lRootGM  -lBaseVGM -lClhepVGM

