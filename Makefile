

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
	-mkdir ~/inf3172
	install -m 0755 tshutils/out/list ~/inf3172/
	install -m 0755 tshutils/out/new ~/inf3172/
	install -m 0755 tshutils/out/newdir ~/inf3172/
	install -m 0755 tshutils/out/rmall ~/inf3172/
	install -m 0755 tshutils/out/size ~/inf3172/
install:
	install -m 0755 tsh/out/tsh turboshell
	-mkdir ~/inf3172
	install -m 0755 tshutils/out/list ~/inf3172/
	install -m 0755 tshutils/out/new ~/inf3172/
	install -m 0755 tshutils/out/newdir ~/inf3172/
	install -m 0755 tshutils/out/rmall ~/inf3172/
	install -m 0755 tshutils/out/size ~/inf3172/
