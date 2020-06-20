SHELL 	= /bin/bash
UTILDIR = libutils
BACKDIR = libbackend
CFLAGS  = -g -Wall -Wextra -pedantic -D_DEFAULT_SOURCE -std=c99

all:	libs backend

clean:
	cd ../frontend; rm -f generated.js generated.data generated.wasm
	cd $(UTILDIR); make clean
	cd $(BACKDIR); make clean

libs:
	cd $(UTILDIR); source ../../emsdk/emsdk_env.sh; emmake make
	cd $(BACKDIR); source ../../emsdk/emsdk_env.sh; emmake make

fetch:
	rm total_cases.csv; wget -nv "https://covid.ourworldindata.org/data/ecdc/total_cases.csv"

backend:
	source ../emsdk/emsdk_env.sh; emcc backend_main.c libbackend/libbackend.a libutils/libutils.a -o ../frontend/generated.js -s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap", "ccall"]' -s ERROR_ON_UNDEFINED_SYMBOLS=1 -s ALLOW_MEMORY_GROWTH=1 -s FETCH=1 -lidbfs.js --preload-file total_cases.csv --preload-file locations.csv --no-heap-copy
