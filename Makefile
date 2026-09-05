.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory

.PHONY: all latex book apps impl exp format test setup clean

all: latex apps impl

format:
	@printf "%s\n" "Formatando todos os códigos C++ (.hpp e .cpp) de todo o repositório com clang-format..."
	find . -type f \( -name "*.hpp" -o -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -not -path "*/.*/*" -exec clang-format -i {} +
	@printf "%s\n" "Formatação completa de todos os códigos C++ concluída com sucesso!"



setup:
	git config core.hooksPath .githooks
	chmod +x .githooks/*

latex:
	$(MAKE) -C LaTeX all

book:
	$(MAKE) -C LaTeX book

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
