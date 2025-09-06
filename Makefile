none:
	echo "none"

prepare_debug:
	conan install . --build=missing -s build_type=Debug
	cmake --preset conan-debug

prepare_release:
	conan install . --build=missing
	cmake --preset conan-release

build_debug: prepare_debug
	cmake --build --preset conan-debug
	
build_release: prepare_release
	cmake --build --preset conan-release

run_debug: build_debug
	cd ./build/Debug/ && ./hex

run_release: build_release
	cd ./build/Release/ && ./hex