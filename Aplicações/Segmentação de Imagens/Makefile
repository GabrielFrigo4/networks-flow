CXX      = c++
CXXFLAGS = -Wl,-z,stack-size=268435456 -std=c++23 -O2
SEG_EXE  = ./segment
SEED_EXE = ./seeds

.PHONY: all seed run test example clean

all: $(SEG_EXE) $(SEED_EXE) seed

$(SEG_EXE): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(SEG_EXE)

$(SEED_EXE): seeds.cpp
	$(CXX) $(CXXFLAGS) seeds.cpp -o $(SEED_EXE)

seed: $(SEED_EXE)
	@for dir in samples/*/; do \
		name=$$(basename "$$dir"); \
		$(SEED_EXE) --txt2ppm --input "$$dir$$name.seeds" --output "$${dir}seeds.ppm" --reference "$$dir$$name.ppm" --ascii; \
	done

run: $(SEG_EXE)
	$(SEG_EXE)

test: $(SEG_EXE)
	@FAILED=0; \
	for dir in samples/*/; do \
		name=$$(basename "$$dir"); \
		input="$$dir$$name.ppm"; \
		seeds="$$dir$$name.seeds"; \
		output="$$dir/output.ppm"; \
		if $(SEG_EXE) --input "$$input" --seeds "$$seeds" --output "$$output" > "/dev/null" 2>&1; then \
			printf "[PASS] $$name\n"; \
		else \
			printf "[FAIL] $$name\n"; \
			FAILED=$$((FAILED+1)); \
		fi; \
	done; \
	exit $$FAILED

example: $(SEG_EXE)
	$(SEG_EXE) --input "samples/grid-all/grid-all.ppm" --seeds "samples/grid-all/grid-all.seeds" --output "samples/grid-all/output.ppm"

clean clear:
	rm -f $(SEG_EXE) $(SEED_EXE) samples/*/output.ppm samples/*/seeds.ppm
