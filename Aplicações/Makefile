.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory

SUBDIRS = "Segmentação de Imagens"

.PHONY: all test clean

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" all; \
	done

test:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" test; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" clean; \
	done
