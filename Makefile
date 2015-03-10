INSTALL_DIR=~/inf3172/bin/

.PHONY: libpathm
.PHONY: tsh
.PHONY: install
.PHONY: build
.PHONY: utils

all: | build install

build: | libpathm utils tsh

libpathm:
	$(MAKE) -C ./libpathm

tsh:
	$(MAKE) -C ./tsh

utils:
	$(MAKE) -C ./tshutils

install:
	install -m 0755 tsh/out/tsh turboshell
	-mkdir -p $(INSTALL_DIR)
	install -m 0755 tshutils/out/list $(INSTALL_DIR)
	install -m 0755 tshutils/out/new $(INSTALL_DIR)
	install -m 0755 tshutils/out/newdir $(INSTALL_DIR)
	install -m 0755 tshutils/out/rmall $(INSTALL_DIR)
	install -m 0755 tshutils/out/size $(INSTALL_DIR)
