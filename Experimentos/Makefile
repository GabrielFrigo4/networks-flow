.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory

.PHONY: all run format clean

all:
	@printf "Módulo de Experimentos configurado. Drivers em desenvolvimento (veja TODO.md).\n"

format:
	@printf "%s\n" "Formatando códigos C++ de Experimentos com clang-format..."
	find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -not -path "*/.*/*" -exec clang-format -i {} +
	@printf "%s\n" "Formatação de Experimentos concluída!"

clean:
	rm -rf results/*.csv results/*.json
