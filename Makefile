###########################################################################
#
# File: Makefile
# Author: Andrew Howard, Richard Vaughan
# Date: 27 Apr 2001
# Desc: Top-level make file for Stage
#
# CVS info:
#  $Source: /home/tcollett/stagecvs/playerstage-cvs/code/stage/Makefile,v $
#  $Author: vaughan $
#  $Revision: 1.13 $
#
###########################################################################

include Makefile.common

##############################################################################
# Top level targets

all: stage xs rtkstage

stage: 
	cd src && make clean && make stage -e PLAYER_DIR=$(PLAYER_DIR) &&  cp stage ../bin/

xs: 
	cd src &&  make xs -e PLAYER_DIR=$(PLAYER_DIR) && cp xs ../bin/

rtkstage:
	cd rtk && make all
	cd src && make clean && make rtkstage -e PLAYER_DIR=$(PLAYER_DIR) &&  cp rtkstage ../bin/

dep:
	cd rtk &&  make dep
	cd src &&  make dep

clean:
	rm -f *~
	cd rtk &&  make clean
	cd src &&  make clean 
	cd include &&  make clean
	cd bin &&  rm -f stage rtkstage xs core


###########################################################################
# Install section

INSTALL_BIN = $(INSTALL_DIR)/bin
INSTALL_BIN_FILES = bin/stage bin/rtkstage bin/xs
INSTALL_EXAMPLES = $(INSTALL_DIR)/examples
INSTALL_ETC = /usr/local/etc/rtk
INSTALL_ETC_FILES = etc/rtkstage.cfg

install:
	mkdir -p $(INSTALL_DIR)/bin
	install -m 755 $(INSTALL_BIN_FILES) $(INSTALL_BIN)
	mkdir -p $(INSTALL_EXAMPLES)
	install -m 644 examples/*.world* $(INSTALL_EXAMPLES)
	install -m 644 examples/*.pnm* $(INSTALL_EXAMPLES)
	mkdir -p $(INSTALL_ETC)
	install -m 755 $(INSTALL_ETC_FILES) $(INSTALL_ETC)


###########################################################################
# Create a source distribution
# Do a make clean and remove extraneous files first.

src_dist:
	echo Building $(SRC_DIST_NAME)
	cp -R . /tmp/$(SRC_DIST_NAME)
	tar -C /tmp -cvzf $(SRC_DIST_NAME).tgz --exclude CVS --exclude '*.tgz' $(SRC_DIST_NAME)
	rm -Rf /tmp/$(SRC_DIST_NAME)

src_dist_bleeding:
	echo Building $(SRC_DIST_BLEEDING_NAME)
	cp -R . /tmp/$(SRC_DIST_BLEEDING_NAME)
	tar -C /tmp -cvzf $(SRC_DIST_BLEEDING_NAME).tgz --exclude CVS --exclude '*.tgz' $(SRC_DIST_BLEEDING_NAME)
	rm -Rf /tmp/$(SRC_DIST_BLEEDING_NAME)

###########################################################################
# Create a binary distribution
# Do a make clean and remove extraneous files first.
# Then do a make all

BIN_DIST_FILES = \
	$(BIN_DIST_NAME)/README.stage \
	$(BIN_DIST_NAME)/VERSION \
	$(BIN_DIST_NAME)/Makefile \
	$(BIN_DIST_NAME)/bin/stage \
	$(BIN_DIST_NAME)/bin/rtkstage \
	$(BIN_DIST_NAME)/etc/rtkstage.cfg  

bin_dist:
	echo Building $(BIN_DIST_NAME)
	cp -R . /tmp/$(BIN_DIST_NAME)
	tar -C /tmp -cvzf $(BIN_DIST_NAME).tgz $(BIN_DIST_FILES)
	rm -Rf /tmp/$(BIN_DIST_NAME)


# DO NOT DELETE









