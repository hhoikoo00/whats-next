'use strict';

/* Load related libraries for Google Charts */
google.charts.load('current', {
    'packages': ['corechart', 'geochart'],
    'mapsApiKey':'AIzaSyD-9tSrke72PouQMnMX-a7eZSW0jkFMBWY'
});
google.charts.setOnLoadCallback(function () {
    /* Functions to call on website load */
     drawWorldMap();
     drawLineGraph('World');
});

function reset() {
    drawLineGraph('World');
}

function getWorldMapDataArray() {
    /* Get an array of country names */
    let countryNamesLongString = ccall("get_all_countries", "string", []);
    let countryNames = countryNamesLongString.split("=");

    /* Get an array of number of cases */
    let numCases = ccallArrays("get_most_updated_cases", "array", [], [], {heapIn: "HEAPU32", heapOut: "HEAPU32", returnArraySize: countryNames.length});

    /* Combine the arrays */
    let resultArray = [['Country', 'Number of Cases']];
    let k = 1;
    for (let i = 0; i <= countryNames.length; i++) {
	if (numCases[i] >= 100) {
	    resultArray[k++] = [countryNames[i], numCases[i]];
	}
    }

    return resultArray;
}

function getLineGraphDataArray(country) {
    const DAY_TO_MILLISECOND = 86400000;
    const NUM_DAYS_TO_PREDICT = 50;

    /* Get number of days in dataset */
    let daysCount = ccall("get_days_in_dataset", "number", []);
    let dayZero = ccall("get_day_zero", "number", ["string"], [country]);

    /* Get the first date */
    let startDateString = ccall("get_start_date", "string", []);
    let startDate = new Date(+startDateString.substring(0, 4),
                             +startDateString.substring(5, 7) - 1,
                             +startDateString.substring(8, 10));
    let firstDate = new Date(+startDate + dayZero * DAY_TO_MILLISECOND);
    
    /* Get an array of predicted and actual number of cases */
    let predictedCases = ccallArrays("get_appended_data", "array", ["string", "number"], [country, NUM_DAYS_TO_PREDICT], {heapIn: "HEAPU32", heapOut: "HEAPU32", returnArraySize: daysCount - dayZero + NUM_DAYS_TO_PREDICT});
    let actualCases = ccallArrays("get_country_data", "array", ["string"], [country], {heapIn: "HEAPU32", heapOut: "HEAPU32", returnArraySize: daysCount - dayZero});

    /* Add values to the array */
    let k = 0;
    let resultArray = [['Date', 'Predicted', 'Actual']];
    for (let i = 1; i <= predictedCases.length; i++) {
	let actual = (i - 1 < actualCases.length) ? actualCases[i - 1] : null;
	resultArray[i] = [new Date(+firstDate + k++ * DAY_TO_MILLISECOND),
			  predictedCases[i - 1], actual];
    }

    return resultArray;
}

function drawWorldMap() {
    /* Get data from a function */
    let data = google.visualization.arrayToDataTable(getWorldMapDataArray());

    /* Define chart options */
    let options = {
	width:$(window).width() * 0.75,
	height:$(window).height() * 0.5,
	colorAxis:{minValue:0, colors:['seagreen', 'firebrick', 'black']}
    };

    let chart = new google.visualization.GeoChart(document.getElementById('worldMap'));

    /* Add an event listener for worldMap that sets the line graph */
    google.visualization.events.addListener(chart, 'select', function () {
	let selection = chart.getSelection();
	let selectedCountry = (selection.length > 0) ?
	    data.getValue(selection[0].row, 0) : 'World';

	/* call function that resets chart */
	drawLineGraph(selectedCountry);
    });

    /* Draw the worldMap */
    chart.draw(data, options);
}

function drawLineGraph(country) {
    /* Get data from a function */
    let data = new google.visualization.arrayToDataTable(getLineGraphDataArray(country));

    /* Define chart options */
    let options = {
	title:'Predicted and Actual Cumulative Number of Cases for ' + country,
	width:$(window).width(),
	height:$(window).height() * 0.75,
	curveType:'function',
	legend:'bottom'
    };

    /* Draw the chart */
    let chart = new google.visualization.LineChart(document.getElementById('lineGraph'));
    chart.draw(data, options);
}
