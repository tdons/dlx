all:
	mkdir -p build/
	cd build && cmake .. && make
.PHONY : all

test: all
	cd build && make test
.PHONY : test

js:
	mkdir -p build-js/
	cd build-js && cmake -DCMAKE_TOOLCHAIN_FILE=$(shell find /usr/local -name Emscripten.cmake) .. && make
.PHONY : js

clangformat:
	find . \( -name '*.c' -or -name '*.h' \) -exec clang-format -style=file -i \{} \;
.PHONY : clangformat

clean:
	rm -rf build/ build-js/
.PHONY : clean