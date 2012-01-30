# --------------------------------------------------------------
# GNUmakefile beta simulation        Whitney Armstrong 12/16/07
# --------------------------------------------------------------
  name := BETA
  G4TARGET := $(name)
  G4EXLIB := true

.PHONY: all

all: lib bin

doc: /home/whit/sane08/InSANE/doc/BETAG4_Doxyfile
	doxygen /home/whit/sane08/InSANE/doc/BETAG4_Doxyfile


include $(G4INSTALL)/config/binmake.gmk

# ROOT
  CPPFLAGS += $(shell root-config --cflags )
  CPPFLAGS += -g
#   LDLIBS += $(shell root-config --glibs) 
   LDLIBS += -lGeomPainter -lGeom -lTreeViewer -lSpectrum -lSpectrumPainter -lFoam  
   LDLIBS +=  $(shell root-config --ldflags)
   LDLIBS += $(shell root-config --glibs --libs ) 
   LDLIBS += -lTreePlayer -lGed -lRGL -lEve -lEG
   LDLIBS += $(shell mysql_config --libs)

#LHAPDF
  LDLIBS += $(shell lhapdf-config --ldflags ) 
  CPPFLAGS += $(shell lhapdf-config --cppflags )


# InSANE 
   LDLIBS += -L$(InSANEDIR)/lib
   LDLIBS += -lInSANE  
   CPPFLAGS += -I$(InSANEDIR)/include

# ROOT and  GNU Scientific Librarires and 
   LDLIBS += -lm  $(shell root-config --nonew --glibs)
   LDLIBS += $(shell gsl-config  --libs)

# VGM
#   CPPFLAGS  += -I$(VGM_INSTALL)/packages/VGM/include
#   CPPFLAGS  += -I$(VGM_INSTALL)/packages/BaseVGM/include
#   CPPFLAGS  += -I$(VGM_INSTALL)/packages/Geant4GM/include
#   CPPFLAGS  += -I$(VGM_INSTALL)/packages/XmlVGM/include
#   CPPFLAGS  += -I$(VGM_INSTALL)/packages/RootGM/include
#   LDLIBS += -L$(VGM_INSTALL)/lib/$(VGM_SYSTEM)
#   LDLIBS += -lGeant4GM -lRootGM  -lBaseVGM -lClhepVGM -lXmlVGM

   CXXFLAGS  += -O2 -Wall -fPIC
   CXXFLAGS  += $(CPPFLAGS)





