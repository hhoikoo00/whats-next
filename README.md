# What’s Next? - Predicting the Spread of the Coronavirus 1

## Introduction

This project was developed as an Extension of the First Year C Project for the Computing students of Imperial College London. The group members were Hyunhoi Koo ([hk619@ic.ac.uk](mailto:hk619@ic.ac.uk)), Jia Qi Poon ([jqp18@ic.ac.uk](mailto:jqp18@ic.ac.uk)), Alexander Goncharov ([ag2719@ic.ac.uk](mailto:ag2719@ic.ac.uk)) and Richard Yang ([ry819@ic.ac.uk](mailto:ry819@ic.ac.uk)).

For this project, we first developed a statistical regression tool in C that predicts the growth of a pandemic using logistic curves. We wrapped this library around a website frontend and created this web program, which shows a prediction of the trend of cumulative number of coronavirus cases with the actual cumulative number of cases.

We obtained a [CSV file](https://covid.ourworldindata.org/data/ecdc/total_cases.csv) that contains the total number of coronavirus cases for 200+ countries. Using this data and our statistical model, the program predicts the trend of the
growth of the coronavirus cases up to 50 days in the future.

## Explanation of the Library

For our statistical analysis, we used a [logistic growth model](https://arxiv.org/abs/2003.05681) as it has has been widely used to model previous pandemics. It reflects the exponential growth during the early stages of an epidemic and tends towards a limit that represents the later stages. We calculated the values of our parameters to best fit the existing data and used them to predict future cases. However, because the real-life factors, such as testing, contact tracing, and other governmental responses, vary greatly between the countries, our model was not accurate for certain countries across the different stages of the pandemic. 

The equation we used to model the pandemic is the logistic growth model. It has been widely used to model previous pandemics such as SARS and Ebola, as it contains a portion of exponential growth to represent the early stages of an epidemic. However, unlike an exponential growth model, the logistic growth model also tends to a maximum limit towards the end of the graph, which corresponds to the virus being successfully contained or infecting most of the population. The equation we used for our logistic growth model is

<img src="https://render.githubusercontent.com/render/math?math=\Large y(t) = \frac{c}{1 %2B a e^{-b t}}">

where c is the maximum number of people infected, b is the scaling factor for the growth rate, and a affects the initial rate of infection when t = 0. To optimize our equation, we had to find values for these three constants, such that our model would be able to produce results as close as possible to the actual data.

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
