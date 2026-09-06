.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory

CXX      = c++
CXXFLAGS = -std=c++23 -fsyntax-only -Wall -Wextra

.PHONY: all check format test clean

all: check

format:
	@printf "%s\n" "Formatando todos os códigos C++ (.hpp e .cpp) em Implementações com clang-format..."
	find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -not -path "*/.*/*" -exec clang-format -i {} +
	@printf "%s\n" "Formatação completa de Implementações concluída!"

check:
	@printf "%s\n" "Verificando sintaxe dos headers (FlowNetwork)..."
	@for f in FlowNetwork/*.hpp; do \
		$(CXX) $(CXXFLAGS) "$$f" || exit 1; \
	done
	@printf "%s\n" "Verificando sintaxe dos headers (CostNetwork)..."
	@for f in CostNetwork/*.hpp; do \
		$(CXX) $(CXXFLAGS) "$$f" || exit 1; \
	done
	@printf "%s\n" "Todos os headers compilaram perfeitamente!"

test:
	@printf "%s\n" "Executando testes dos problemas de Fluxo Máximo..."
	@for dir in Problemas/"Fluxo Máximo"/*/; do \
		case "$$dir" in *Template*) continue ;; esac; \
		if [ -f "$${dir}Makefile" ]; then \
			printf "%s\n" "-> Testando $$dir"; \
			$(MAKE) -C "$$dir" all > /dev/null || exit 1; \
		fi \
	done
	@printf "%s\n" "Executando testes dos problemas de Fluxo de Custo Mínimo..."
	@for dir in Problemas/"Fluxo de Custo Mínimo"/*/; do \
		case "$$dir" in *Template*) continue ;; esac; \
		if [ -f "$${dir}Makefile" ]; then \
			printf "%s\n" "-> Testando $$dir"; \
			$(MAKE) -C "$$dir" all > /dev/null || exit 1; \
		fi \
	done
	@printf "%s\n" "Todos os problemas foram executados com sucesso!"

clean:
	@for dir in Problemas/"Fluxo Máximo"/*/ Problemas/"Fluxo de Custo Mínimo"/*/; do \
		if [ -f "$${dir}Makefile" ]; then \
			$(MAKE) -C "$$dir" clean; \
		fi \
	done
