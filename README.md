# What’s Next? - A Coronavirus Prediction

## Getting Started

### Prerequisites
##### To run web server
- `python 2` OR `python 3`

##### To compile WebAssembly
- [`Emscripten`](https://github.com/emscripten-core/emscripten)

### Usage
##### To compile WebAssembly
1. Follow instructions [here](https://emscripten.org/docs/getting_started/downloads.html) to download and install latest version of Emscripten SDK (emsdk)

2. Copy emsdk folder to the same Extension as shown

```
Extension
│
└───backend
│   │
│   └─── ...
│   
└───frontend
│   │
│   └─── ...
|
└───emsdk
...
```

3. Run `make` in backend folder

`$ cd backend && make`

##### To run web server
1. Execute `run_server.sh` in frontend folder

`$ cd frontend && ./run_server.sh`

2. The server should run on the default port, 8000. Open your favourite internet browser and go to `http://0.0.0.0:8000/`
