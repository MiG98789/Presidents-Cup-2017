window.onload = function() {
    var video = document.getElementById('video');
    var canvas = document.getElementById('canvas');
    var context = canvas.getContext('2d');
    var currFaceCount = [0];
    var totalFaceCount = [];
    var countIndex = 0;

    var bars = d3.select(".chart")
        .selectAll("div")
        .data(currFaceCount);

    // Set up the bar chart
    bars.enter().append("div")
        .style("width", function(d) { return d*50 + "px"; })
        .text(function(d) { return d; });

    // Set up the dot plot
    var margin = {top: 20, right: 20, bottom: 30, left: 40},
        width = 960 - margin.left - margin.right,
        height = 500 - margin.top - margin.bottom;

    // Set up x
    var xScale = d3.scaleLinear().range([0, width]),
        xAxis = d3.axisBottom().scale(xScale);

    // Set up y
    var yScale = d3.scaleLinear().range([height, 0]),
        yAxis = d3.axisLeft().scale(yScale);

    // setup fill color
    var cValue = function(d) { return d[countIndex];},
        color = d3.scaleOrdinal(d3.schemeCategory10);

    // Add the graph canvas to the body of the webpage
    var svg = d3.select(".plot").append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
        .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

    // Don't want dots overlapping axis, so add in buffer to data domain
    xScale.domain([-1, 450]);
    yScale.domain([-1, 10]);

    // x-axis
    svg.append("g")
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis)
        .append("text")
        .attr("class", "label")
        .attr("x", width)
        .attr("y", -6)
        .attr("text-anchor", "end")
        .text("Time");

    // y-axis
    svg.append("g")
        .attr("class", "y axis")
        .call(yAxis)
        .append("text")
        .attr("class", "label")
        .attr("transform", "rotate(-90)")
        .attr("y", 6)
        .attr("dy", ".75em")
        .attr("text-anchor", "end")
        .text("Number of Faces");

    // Set up face tracker
    var tracker = new tracking.ObjectTracker('face');
    tracker.setInitialScale(1);
    tracker.setStepSize(1);
    tracker.setEdgesDensity(0.1);
    tracking.track('#video', tracker, { camera: true });

    // Tracker loop function
    tracker.on('track', function(event) {
        context.clearRect(0, 0, canvas.width, canvas.height);
        currFaceCount[0] = 0;
        bars.style("width", function (d) {return d*50 + "px";})
            .text(function (d) {return d;});

        event.data.forEach(function(rect) { // Loop through each detected face
            currFaceCount[0] += 1;

            // Draw a rectangle around the detected face
            context.strokeStyle = '#a64ceb';
            context.strokeRect(rect.x, rect.y, rect.width, rect.height);
            context.font = '11px Helvetica';
            context.fillStyle = "#fff";
            context.fillText('x: ' + rect.x + 'px', rect.x + rect.width + 5, rect.y + 11);
            context.fillText('y: ' + rect.y + 'px', rect.x + rect.width + 5, rect.y + 22);
        });

        totalFaceCount.push(currFaceCount[0]);

        bars = d3.select(".chart")
            .selectAll("div")
            .attr("id", "chart")
            .data(currFaceCount);

        // Enter selection
        bars.enter().append("div");

        // Update selection
        bars
            .style("width", function (d) {return d*50 + "px";})
            .text(function (d) {return d;});

        // Exit selection
        bars.exit().remove();

        // Don't want dots overlapping axis, so add in buffer to data domain
        xScale.domain([-1, 450]);
        yScale.domain([d3.min(totalFaceCount)-1, d3.max(totalFaceCount)]);

        svg.selectAll("g.y.axis").call(yAxis);
        svg.selectAll("g.x.axis").call(xAxis);

        // Update dot plot
        svg.selectAll(".dot")
            .data(totalFaceCount)
            .enter().append("circle")
            .attr("class", "dot")
            .attr("r", 3.5)
            .attr("cx", xScale(countIndex))
            .attr("cy", yScale(totalFaceCount[countIndex]))
            .style("fill", function(d) { return color(cValue(d));});

        countIndex++;
    });

    /*
     var gui = new dat.GUI();
     gui.add(tracker, 'edgesDensity', 0.1, 0.5).step(0.01);
     gui.add(tracker, 'initialScale', 1.0, 10.0).step(0.1);
     gui.add(tracker, 'stepSize', 1, 5).step(0.1);
     */
};