.POSIX:
.SILENT:

MAKEFLAGS += --no-print-directory -s

# ----------------------------------------------------------------
# Makefile: Network Flow Implementations
# ----------------------------------------------------------------

CXX      = c++
CXXFLAGS = -std=c++23 -fsyntax-only -Wall -Wextra

.PHONY: default help all check impl test verify format format-cpp format-md clean clear

default: help
all: check

### ================================
### HELP & DOCUMENTATION
### ================================
help:
	_e=$$'\e'; \
	cmd() { printf "    $${_e}[36mmake %-20s$${_e}[0m %s\n" "$$1" "$$2"; }; \
	sec() { printf "\n  $${_e}[1;33m%s$${_e}[0m\n" "$$1"; }; \
	printf "\n  $${_e}[1;37mNetwork Flow — Módulo de Implementações em C++23$${_e}[0m\n"; \
	printf "  ============================================================\n"; \
	sec "Fluxos Batch & Validação:"; \
	cmd "verify"              "Executa checagem de sintaxe dos headers E todos os testes de problemas"; \
	cmd "check"               "Verifica a sintaxe C++23 de todos os headers (FlowNetwork e CostNetwork)"; \
	cmd "impl"                "Sinônimo para check (valida sintaxe C++23 dos headers)"; \
	cmd "test"                "Executa os problemas de Fluxo Máximo e Fluxo de Custo Mínimo"; \
	sec "Qualidade & Formatação:"; \
	cmd "format"              "Formata códigos C++ (clang-format) e documentação (prettier)"; \
	sec "Limpeza:"; \
	cmd "clean"               "Remove binários compilados das subpastas de problemas"; \
	echo ""

verify: check test
	printf "\n$${_e}[1;32m==> Todas as implementações e problemas foram validados com sucesso!$${_e}[0m\n\n"

format: format-cpp format-md
	printf "%s\n" "Formatação completa de Implementações concluída!"

format-cpp:
	printf "%s\n" "Formatando todos os códigos C++ (.hpp e .cpp) em Implementações com clang-format..."
	find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -not -path "*/.*/*" -exec clang-format -i {} +

format-md:
	if command -v prettier > "/dev/null" 2>&1; then \
		printf "%s\n" "Formatando documentação Markdown de Implementações com prettier..."; \
		prettier --write "**/*.md" > "/dev/null" 2>&1 || true; \
	fi

check:
	printf "%s\n" "Verificando sintaxe dos headers (FlowNetwork)..."
	for f in FlowNetwork/*.hpp; do \
		$(CXX) $(CXXFLAGS) "$$f" || exit 1; \
	done
	printf "%s\n" "Verificando sintaxe dos headers (CostNetwork)..."
	for f in CostNetwork/*.hpp; do \
		$(CXX) $(CXXFLAGS) "$$f" || exit 1; \
	done
	printf "%s\n" "Todos os headers compilaram perfeitamente!"

impl: check

test:
	printf "%s\n" "Executando testes dos problemas de Fluxo Máximo..."
	for dir in Problemas/"Fluxo Máximo"/*/; do \
		case "$$dir" in *Template*) continue ;; esac; \
		if [ -f "$${dir}Makefile" ]; then \
			printf "%s\n" "-> Testando $$dir"; \
			$(MAKE) -C "$$dir" all > "/dev/null" || exit 1; \
		fi \
	done
	printf "%s\n" "Executando testes dos problemas de Fluxo de Custo Mínimo..."
	for dir in Problemas/"Fluxo de Custo Mínimo"/*/; do \
		case "$$dir" in *Template*) continue ;; esac; \
		if [ -f "$${dir}Makefile" ]; then \
			printf "%s\n" "-> Testando $$dir"; \
			$(MAKE) -C "$$dir" all > "/dev/null" || exit 1; \
		fi \
	done
	printf "%s\n" "Todos os problemas foram executados com sucesso!"

clean:
	for dir in Problemas/"Fluxo Máximo"/*/ Problemas/"Fluxo de Custo Mínimo"/*/; do \
		if [ -f "$${dir}Makefile" ]; then \
			$(MAKE) -C "$$dir" clean; \
		fi \
	done

clear: clean
