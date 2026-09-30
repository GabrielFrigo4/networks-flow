.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory -s

CXX      = c++
CXXFLAGS = -Wl,-z,stack-size=268435456 -std=c++23 -O2
SEG_EXE  = ./segment
SEED_EXE = ./seeds
PPM_EXE  = ./ppm

.PHONY: default help all build verify seed run test example format format-cpp format-md clean clear

default: help
all: build seed

### ================================
### HELP & DOCUMENTATION
### ================================
help:
	cmd() { printf "    \033[36mmake %-20s\033[0m %s\n" "$$1" "$$2"; }; \
	sec() { printf "\n  \033[1;33m%s\033[0m\n" "$$1"; }; \
	printf "\n  \033[1;37mSegmentação de Imagens via Corte Mínimo (C++23)\033[0m\n"; \
	printf "  ============================================================\n"; \
	sec "Fluxos Batch & Execução:"; \
	cmd "verify"              "Compila executáveis, gera seeds PPM e roda bateria de testes"; \
	cmd "build"               "Compila os utilitários: segment, seeds e ppm"; \
	cmd "seed"                "Converte os arquivos de seeds (.seeds) para imagens (.ppm)"; \
	cmd "test"                "Executa os casos de teste e valida saídas contra o gabarito"; \
	cmd "example"             "Executa segmentação no exemplo canônico samples/grid1"; \
	sec "Qualidade & Formatação:"; \
	cmd "format"              "Formata códigos C++ (clang-format) e documentação (prettier)"; \
	sec "Limpeza:"; \
	cmd "clean"               "Remove binários e arquivos PPM gerados intermediários"; \
	echo ""

verify: build seed test
	printf "\n\033[1;32m==> Segmentação de Imagens compilada e testada com sucesso!\033[0m\n\n"

build: $(SEG_EXE) $(SEED_EXE) $(PPM_EXE)

format: format-cpp format-md
	printf "%s\n" "Formatação da Segmentação de Imagens concluída!"

format-cpp:
	printf "%s\n" "Formatando códigos C++ da Segmentação de Imagens com clang-format..."
	find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -not -path "*/.*/*" -exec clang-format -i {} +

format-md:
	if command -v prettier > "/dev/null" 2>&1; then \
		printf "%s\n" "Formatando documentação Markdown com prettier..."; \
		prettier --write "**/*.md" > "/dev/null" 2>&1 || true; \
	fi

$(SEG_EXE): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(SEG_EXE)

$(SEED_EXE): seeds.cpp
	$(CXX) $(CXXFLAGS) seeds.cpp -o $(SEED_EXE)

$(PPM_EXE): ppm.cpp
	$(CXX) $(CXXFLAGS) ppm.cpp -o $(PPM_EXE)

seed: $(SEED_EXE)
	for dir in samples/*/; do \
		$(SEED_EXE) --txt2ppm --input "$${dir}source.seeds" --output "$${dir}seeds.ppm" --reference "$${dir}source.ppm" --ascii; \
	done

run: $(SEG_EXE)
	$(SEG_EXE)

test: $(SEG_EXE) $(PPM_EXE)
	FAILED=0; \
	for dir in samples/*/; do \
		name=$$(basename "$$dir"); \
		if $(SEG_EXE) --input "$${dir}source.ppm" --seeds "$${dir}source.seeds" --output "$${dir}output.ppm" > "/dev/null" 2>&1; then \
			$(PPM_EXE) --input "$${dir}goal.ppm"   --output "$${dir}goal.tmp.ppm"   --binary > "/dev/null" 2>&1; \
			$(PPM_EXE) --input "$${dir}output.ppm" --output "$${dir}output.tmp.ppm" --binary > "/dev/null" 2>&1; \
			if cmp -s "$${dir}output.tmp.ppm" "$${dir}goal.tmp.ppm"; then \
				printf "[PASS] %s\n" "$$name"; \
			else \
				printf "[FAIL] %s (outputs differ)\n" "$$name"; \
				FAILED=$$((FAILED+1)); \
			fi; \
			rm -f "$${dir}output.tmp.ppm" "$${dir}goal.tmp.ppm"; \
		else \
			printf "[FAIL] %s (execution failed)\n" "$$name"; \
			FAILED=$$((FAILED+1)); \
		fi; \
	done; \
	exit $$FAILED

example: $(SEG_EXE)
	$(SEG_EXE) --input "samples/grid1/source.ppm" --seeds "samples/grid1/source.seeds" --output "samples/grid1/output.ppm"

clean clear:
	rm -f $(SEG_EXE) $(SEED_EXE) $(PPM_EXE) samples/*/output.ppm samples/*/seeds.ppm
