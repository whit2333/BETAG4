# --------------------------------------------------------------
# GNUmakefile beta simulation        Whitney Armstrong 12/16/07
# --------------------------------------------------------------

  name := BETA
  G4TARGET := $(name)
  G4EXLIB := true

.PHONY: all

all: lib bin

include $(G4INSTALL)/config/binmake.gmk


#   CPPFLAGS += `aida-config --include `
#   LDLIBS += `aida-config --lib `
# ROOT
   CPPFLAGS += $(shell root-config --cflags )
   CPPFLAGS += -g -I$(shell root-config --incdir )
   LDLIBS += $(shell root-config --glibs) 
   LDLIBS += -lGeomPainter -lGeom -lSpectrum -lSpectrumPainter
   LDLIBS +=  $(shell root-config --ldflags)
   LDLIBS += $(shell gsl-config --libs) $(shell root-config --glibs --libs ) 
#   LDLIBS += -lTreePlayer -lGed -lRGL -lEve -lEG
   LDLIBS += $(shell mysql_config --libs)

# InSANE 
   LDLIBS += -L$(InSANEDIR)/lib
   LDLIBS += -lInSANE  
   CPPFLAGS += -I$(InSANEDIR)/include
# ROOT and  GNU Scientific Librarires and 
   LDLIBS += -lgsl -lgslcblas -lm  $(shell root-config --nonew --glibs)

# VGM
   CPPFLAGS  += -I$(VGM_INSTALL)/packages/VGM/include
   CPPFLAGS  += -I$(VGM_INSTALL)/packages/BaseVGM/include
   CPPFLAGS  += -I$(VGM_INSTALL)/packages/Geant4GM/include
   CPPFLAGS  += -I$(VGM_INSTALL)/packages/RootGM/include

#   LDLIBS += -L$(VGM_INSTALL)/lib/$(VGM_SYSTEM)
#   LDLIBS += -lGeant4GM -lRootGM  -lBaseVGM -lClhepVGM 
#CXXFLAGS  += -O2 -Wall -fPIC
#CXXFLAGS  += $(CPPFLAGS)


doc:
	doxygen /home/whit/sane08/documentation/Doxyfile_BETAG4

rootdict: 


