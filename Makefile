.POSIX:

.PHONY: all latex apps impl exp test setup clean

all: latex apps impl

setup:
	git config core.hooksPath .githooks
	chmod +x .githooks/*

latex:
	$(MAKE) -C LaTeX all

apps:
	$(MAKE) -C Aplicações all

impl:
	$(MAKE) -C Implementações all

exp:
	$(MAKE) -C Experimentos all

test:
	$(MAKE) -C Implementações test
	$(MAKE) -C Aplicações test

clean:
	$(MAKE) -C LaTeX distclean
	$(MAKE) -C Aplicações clean
	$(MAKE) -C Implementações clean
	$(MAKE) -C Experimentos clean
