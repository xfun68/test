#=============================================================================== {{{
#
#     Filename: Makefile
#  Description:
#
#        Usage: make              (generate executable                      )
#               make clean        (remove objects, executable, prerequisits )
#               make tarball      (generate compressed archive              )
#               make zip          (generate compressed archive              )
#
#      Version: 1.0
#      Created:
#     Revision: ---
#
#       Author:
#      Company:
#        Email:
#
#        Notes: This is a GNU make (gmake) makefile.
#               C   extension   :  c
#               C++ extensions  :  cc cpp C
#               C and C++ sources can be mixed.
#               Prerequisites are generated automatically; makedepend is not
#               needed (see documentation for GNU make Version 3.80, July 2002,
#               section 4.13). The utility sed is used.
#========================================== makefile template version 1.8 ====== }}}

# LIB can be set to YES to build a library not a executable program, or no otherwise
LIB        		  := NO
LIB_STYLE      	:= STATIC
VER_NUM         := 1
MINOR_NUM       := 1
RELEASE_NUM     := 090524

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          	:= YES

# ------------  name of the target  ----------------------------------------
PROGRAM_NAME    := gatesvr

# ------------  list of all source files  --------------------------------------
SOURCES         := gatesvr.cpp \
		ipc_msg_queue.cpp

# ------------  other name of the target  ----------------------------------------
ifeq (YES,${LIB})
ifeq (SHARE, ${LIB_STYLE})
  LINKER_NAME   := lib${PROGRAM_NAME}.so
  SONAME        := ${LINKER_NAME}.${VER_NUM}
  REAL_NAME     := ${SONAME}.${MINOR_NUM}.${RELEASE_NUM}
else
  LINKER_NAME   := lib${PROGRAM_NAME}.a
  REAL_NAME     := ${LINKER_NAME}
endif
else
  REAL_NAME     := ${PROGRAM_NAME}
endif

# ------------  tools  ---------------------------------------------------------
CC              := gcc
CXX             := g++
AR              := ar rcsv
INSTALL 				:= /usr/bin/install -c
INSTALL_LIB 		:= ${INSTALL} -m 755
INSTALL_PROGRAM := ${INSTALL} -m 755

# ------------  compiler flags  ------------------------------------------------
DEBUG_CFLAGS      := -DLINUX -Wall -ansi -pedantic -O0 -g
DEBUG_CXXFLAGS    := -DLINUX -Wall -ansi -pedantic -O0 -g
RELEASE_CFLAGS    := -DLINUX -Wall -ansi -pedantic -O3
RELEASE_CXXFLAGS  := -DLINUX -Wall -ansi -pedantic -O3

# ------------  linker flags  --------------------------------------------------
DEBUG_LDFLAGS   := -g
RELEASE_LDFLAGS := -g

ifeq (YES,${LIB})
ifeq (SHARE,${LIB_STYLE})
  DEBUG_CFLAGS 			:= ${DEBUG_CFLAGS} -fPIC
  DEBUG_CXXFLAGS    := ${DEBUG_CXXFLAGS} -fPIC
  DEBUG_LDFLAGS     := ${DEBUG_LDFLAGS} -shared -Wl,-soname,${SONAME}
  RELEASE_CFLAGS    := ${RELEASE_CFLAGS} -fPIC
  RELEASE_CXXFLAGS  := ${RELEASE_CXXFLAGS} -fPIC
  RELEASE_LDFLAGS   := ${RELEASE_LDFLAGS} -shared -Wl,-soname,${SONAME}
endif
endif

ifeq (YES, ${DEBUG})
  CFLAGS       := ${DEBUG_CFLAGS}
  CXXFLAGS     := ${DEBUG_CXXFLAGS}
  LDFLAGS      := ${DEBUG_LDFLAGS}
else
  CFLAGS       := ${RELEASE_CFLAGS}
  CXXFLAGS     := ${RELEASE_CXXFLAGS}
  LDFLAGS      := ${RELEASE_LDFLAGS}
endif

# PROFILE can be set to yes to include profiling info, or no otherwise
PROFILE        	:= NO

ifeq (YES, ${PROFILE})
  CFLAGS       := ${CFLAGS}   -pg -O3
  CXXFLAGS     := ${CXXFLAGS} -pg -O3
  LDFLAGS      := ${LDFLAGS}  -pg
endif

# ------------  additional system include directories  -------------------------
GLOBAL_INC_DIR  = $(HOME)/src/base \
									$(ESUITE_HOME)/include

# ------------  private include directories  -----------------------------------
LOCAL_INC_DIR   =

# ------------  system libraries  (e.g. -lm )  ---------------------------------
SYS_LIBS        = pthread \
									m \
									c

# ------------  additional system library directories  -------------------------
GLOBAL_LIB_DIR  = $(ESUITE_HOME)/lib \
									$(HOME)/src/base

# ------------  additional system libraries  -----------------------------------
GLOBAL_LIBS     = EProtocol \
									EAlgorithm \
									base

# ------------  private library directories  -----------------------------------
LOCAL_LIB_DIR   =

# ------------  private libraries  (e.g. libxyz.a )  ---------------------------
LOCAL_LIBS      =

# ------------  archive generation ---------------------------------------------
TARBALL_EXCLUDE = *.{o,gz,zip}
ZIP_EXCLUDE     = *.{o,gz,zip}

# ------------  run executable out of this Makefile  (yes/no)  -----------------
# ------------  cmd line parameters for this executable  -----------------------
EXE_START       = no
EXE_CMDLINE     =

#===============================================================================
# The following statements usually need not to be changed
#===============================================================================

C_SOURCES       = $(filter     %.c, $(SOURCES))
CPP_SOURCES     = $(filter-out %.c, $(SOURCES))
ALL_INC_DIR     = $(addprefix -I, $(LOCAL_INC_DIR) $(GLOBAL_INC_DIR))
ALL_LIB_DIR     = $(addprefix -L, $(LOCAL_LIB_DIR) $(GLOBAL_LIB_DIR))
SYS_LIBSS    		= $(addprefix -l, $(SYS_LIBS))
GLOBAL_LIBSS    = $(addprefix -l, $(GLOBAL_LIBS))
LOCAL_LIBSS     = $(addprefix -l, $(LOCAL_LIBS))
ALL_CFLAGS      = $(CFLAGS) $(ALL_INC_DIR)
ALL_LFLAGS      = $(LDFLAGS) $(ALL_LIB_DIR)
BASENAMES       = $(basename $(SOURCES))

# ------------  generate the names of the object files  ------------------------
OBJECTS         = $(addsuffix .o,$(BASENAMES))

# ------------  generate the names of the hidden prerequisite files  -----------
#PREREQUISITES   = $(addprefix .,$(addsuffix .d,$(BASENAMES)))

# ------------  make goal  ------------------------------------------------
all:	$(REAL_NAME)

$(REAL_NAME):	$(OBJECTS)
ifeq (YES, ${LIB})
ifeq (SHARE, ${LIB_STYLE})
ifeq ($(strip $(CPP_SOURCES)),)
				$(CC)  $(ALL_LFLAGS) -o $(REAL_NAME) $(OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBSS)
else
				$(CXX) $(ALL_LFLAGS) -o $(REAL_NAME) $(OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBSS)
endif
else
				$(AR) $(REAL_NAME) $(OBJECTS)
endif
else
ifeq ($(strip $(CPP_SOURCES)),)
				$(CC)  $(ALL_LFLAGS) -o $(REAL_NAME) $(OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBSS)
else
				$(CXX) $(ALL_LFLAGS) -o $(REAL_NAME) $(OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBSS)
endif
endif

#-------------  Æô¶¯³ÌÐò  ------------------------------------------------------
ifeq ($(EXE_START),yes)
								./$(REAL_NAME) $(EXE_CMDLINE)
endif

# ------------  include the automatically generated prerequisites  -------------
# ------------  if target is not clean, tarball or zip             -------------
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),tarball)
ifneq ($(MAKECMDGOALS),zip)
# include         $(PREREQUISITES)
endif
endif
endif

# ------------  make the objects  ----------------------------------------------
%.o:		%.c
				$(CC)  -c $(ALL_CFLAGS) $<

%.o:		%.cc
				$(CXX) -c $(ALL_CFLAGS) $<

%.o:		%.cpp
				$(CXX) -c $(ALL_CFLAGS) $<

%.o:		%.C
				$(CXX) -c $(ALL_CFLAGS) $<

# ------------  make the prerequisites  ----------------------------------------
#
.%.d:   %.c
				@$(make-prerequisite-c)

.%.d:		%.cc
				@$(make-prerequisite-cplusplus)

.%.d:		%.cpp
				@$(make-prerequisite-cplusplus)

.%.d:		%.C
				@$(make-prerequisite-cplusplus)

#  canned command sequences
#  echoing of the sed command is suppressed by the leading @

define	make-prerequisite-c
				@$(CC)   -MM $(ALL_CFLAGS) $< > $@.$$$$;            \
				sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
				rm -f $@.$$$$;
endef

define	make-prerequisite-cplusplus
				@$(CXX)  -MM $(ALL_CFLAGS) $< > $@.$$$$;            \
				sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
				rm -f $@.$$$$;
endef

# ------------  install  -------------------------------------------------------
PREFIX 							 := $(HOME)
BINDIR_RELATIVE 		 := bin
LIBDIR_RELATIVE 		 := lib
BINDIR = $(PREFIX)/$(BINDIR_RELATIVE)
LIBDIR = $(PREFIX)/$(LIBDIR_RELATIVE)

install:
ifeq (YES, ${LIB})
				mkdir -p $(LIBDIR)
				$(INSTALL_LIB) $(REAL_NAME) $(LIBDIR)
				# cd $(LIBDIR)
				# rm -f $(LINKER_NAME) $(SONAME)
				# /sbin/ldconfig -nv $(LIBDIR)
				# ln -sf $(SONAME) $(LINKER_NAME)
else
				mkdir -p $(BINDIR)
				$(INSTALL_PROGRAM) $(REAL_NAME) $(BINDIR)
endif

# ------------  remove generated files  ----------------------------------------
# ------------  remove hidden backup files  ------------------------------------
#-rm  --force  $(REAL_NAME) $(OBJECTS) $(PREREQUISITES) *~
clean:
								-rm  --force  $(REAL_NAME) $(OBJECTS) *~


# ------------ tarball generation ----------------------------------------------
tarball:
					@lokaldir=`pwd`; lokaldir=$${lokaldir##*/}; \
					rm --force $$lokaldir.tar.gz;               \
					tar --exclude=$(TARBALL_EXCLUDE)            \
					    --create                                \
					    --gzip                                  \
					    --verbose                               \
					    --file  $$lokaldir.tar.gz *

# ------------ zip -------------------------------------------------------------
zip:
					@lokaldir=`pwd`; lokaldir=$${lokaldir##*/}; \
					zip -r  $$lokaldir.zip * -x $(ZIP_EXCLUDE)

.PHONY: clean tarball zip

# ==============================================================================
# vim: set tabstop=2: set shiftwidth=2:
