SHELL := /bin/bash

.PHONY: all
all: setup_environment compile run clean

.PHONY: setup_environment
setup_environment:
	@echo "====================================="
	@echo "===== Running environment setup ====="
	@echo "====================================="

	@/opt/nfs/config/cluster204-test-nodes.sh
	@/opt/nfs/config/station204_name_list.sh 1 16 > nodes

	@mpicxx_path=$$(which mpicxx); \
	expected_mpicxx_path="/opt/nfs/mpich-4.0.1/bin/mpicxx" ; \
	if [ "$$mpicxx_path" != "$$expected_mpicxx_path" ]; then \
		echo "Error: mpicxx path does not match the expected path."; \
		echo "Actual path: $$mpicxx_path"; \
		echo "Expected path: $$expected_mpicxx_path"; \
		echo "Please run command: source /opt/nfs/config/source_mpich401.sh"; \
		exit 1; \
	fi

.PHONY: compile
compile: main.cpp src/Matrix.cpp src/LuSolver.cpp include/Matrix.h include/LuSolver.h
	@mpicxx -I include main.cpp src/Matrix.cpp src/LuSolver.cpp -o lu_decomposition

.PHONY: run
run:
	@echo "====================================="
	@echo "========== Running program =========="
	@echo "====================================="

	@num_nodes=$$(wc -l < nodes); \
	mpiexec -f nodes -n $$num_nodes ./lu_decomposition

.PHONY: clean
clean:
	@rm -f lu_decomposition nodes

.PHONY: clean_results
clean_results:
	@rm -f results/L.txt results/U.txt