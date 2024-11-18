.PHONY: all qmake cmake clean clean-qmake clean-cmake run run-qmake run-cmake

all: qmake

qmake:
	mkdir -p build/qmake
	cd build/qmake && qmake ../../RaDoTech.pro
	cd build/qmake && $(MAKE)

cmake:
	mkdir -p build/cmake
	cd build/cmake && cmake ../..
	cd build/cmake && $(MAKE)

run-qmake:
	@if [ -f build/qmake/RaDoTech ]; then \
		echo "Running RaDoTech built with qmake:"; \
		build/qmake/RaDoTech; \
	else \
		echo "Executable not found. Build the project using 'make qmake' first."; \
	fi

run-cmake:
	@if [ -f build/cmake/RaDoTech ]; then \
		echo "Running RaDoTech built with cmake:"; \
		build/cmake/RaDoTech; \
	else \
		echo "Executable not found. Build the project using 'make cmake' first."; \
	fi

run: run-qmake

clean: clean-qmake clean-cmake

clean-qmake:
	@if [ -d build/qmake ]; then \
		echo "Cleaning qmake build..."; \
		cd build/qmake && $(MAKE) clean; \
		rm -rf build/qmake/*; \
	else \
		echo "No qmake build to clean."; \
	fi

clean-cmake:
	@if [ -d build/cmake ]; then \
		echo "Cleaning cmake build..."; \
		cd build/cmake && $(MAKE) clean; \
		rm -rf build/cmake/*; \
	else \
		echo "No cmake build to clean."; \
	fi
