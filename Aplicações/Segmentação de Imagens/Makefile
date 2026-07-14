CXX      = c++
CXXFLAGS = -Wl,-z,stack-size=268435456 -std=c++23 -O2
SEG_EXE  = ./segment
SEED_EXE = ./seeds
PPM_EXE  = ./ppm

.PHONY: all seed run test example clean

all: $(SEG_EXE) $(SEED_EXE) $(PPM_EXE) seed

$(SEG_EXE): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(SEG_EXE)

$(SEED_EXE): seeds.cpp
	$(CXX) $(CXXFLAGS) seeds.cpp -o $(SEED_EXE)

$(PPM_EXE): ppm.cpp
	$(CXX) $(CXXFLAGS) ppm.cpp -o $(PPM_EXE)

seed: $(SEED_EXE)
	@for dir in samples/*/; do \
		$(SEED_EXE) --txt2ppm --input "$${dir}source.seeds" --output "$${dir}seeds.ppm" --reference "$${dir}source.ppm" --ascii; \
	done

run: $(SEG_EXE)
	$(SEG_EXE)

test: $(SEG_EXE) $(PPM_EXE)
	@FAILED=0; \
	for dir in samples/*/; do \
		name=$$(basename "$$dir"); \
		if $(SEG_EXE) --input "$${dir}source.ppm" --seeds "$${dir}source.seeds" --output "$${dir}output.ppm" > "/dev/null" 2>&1; then \
			$(PPM_EXE) "$${dir}goal.ppm" "$${dir}goal.tmp.ppm" binary > "/dev/null" 2>&1; \
			$(PPM_EXE) "$${dir}output.ppm" "$${dir}output.tmp.ppm" binary > "/dev/null" 2>&1; \
			if cmp -s "$${dir}output.tmp.ppm" "$${dir}goal.tmp.ppm"; then \
				printf "[PASS] $$name\n"; \
			else \
				printf "[FAIL] $$name (outputs differ)\n"; \
				FAILED=$$((FAILED+1)); \
			fi; \
			rm -f "$${dir}output.tmp.ppm" "$${dir}goal.tmp.ppm"; \
		else \
			printf "[FAIL] $$name (execution failed)\n"; \
			FAILED=$$((FAILED+1)); \
		fi; \
	done; \
	exit $$FAILED

example: $(SEG_EXE)
	$(SEG_EXE) --input "samples/grid-all/source.ppm" --seeds "samples/grid-all/source.seeds" --output "samples/grid-all/output.ppm"

clean clear:
	rm -f $(SEG_EXE) $(SEED_EXE) $(PPM_EXE) samples/*/output.ppm samples/*/seeds.ppm
