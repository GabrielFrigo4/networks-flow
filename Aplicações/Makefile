.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory

SUBDIRS = "Segmentação de Imagens"

.PHONY: all test format clean

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" all; \
	done

format:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" format; \
	done

test:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" test; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" clean; \
	done
