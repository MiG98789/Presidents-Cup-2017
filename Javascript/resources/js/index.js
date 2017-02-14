window.onload = function() {
    var video = document.getElementById('video');
    var canvas = document.getElementById('canvas');
    var context = canvas.getContext('2d');
    var count = document.getElementById('count');
    var numOfFaces = [0];
    count.value = 0;

    d3.select(".chart")
        .selectAll("div")
        .data(numOfFaces)
        .enter().append("div")
        .style("width", function(d) { return d*50 + "px"; })
        .text(function(d) { return d; });

    var bars = d3.select(".chart")
        .selectAll("div")
        .data(numOfFaces);

    var tracker = new tracking.ObjectTracker('face');
    tracker.setInitialScale(1);
    tracker.setStepSize(1);
    tracker.setEdgesDensity(0.1);
    tracking.track('#video', tracker, { camera: true });

    tracker.on('track', function(event) {
        context.clearRect(0, 0, canvas.width, canvas.height);
        numOfFaces[0] = 0;
        count.value = 0;
        d3.select(".chart")
            .selectAll("div")
            .data(numOfFaces)
            .style("width", function (d) {return d*50 + "px";})
            .text(function (d) {return d;});

        event.data.forEach(function(rect) {
            numOfFaces[0] += 1;
            count.value = numOfFaces[0];

            bars = d3.select(".chart")
                .selectAll("div")
                .attr("id", "chart")
                .data(numOfFaces);

            // enter selection
            bars.enter().append("div");

            // update selection
            bars
                .style("width", function (d) {return d*50 + "px";})
                .text(function (d) {return d;});

            // exit selection
            bars.exit().remove();

            context.strokeStyle = '#a64ceb';
            context.strokeRect(rect.x, rect.y, rect.width, rect.height);
            context.font = '11px Helvetica';
            context.fillStyle = "#fff";
            context.fillText('x: ' + rect.x + 'px', rect.x + rect.width + 5, rect.y + 11);
            context.fillText('y: ' + rect.y + 'px', rect.x + rect.width + 5, rect.y + 22);
        });
    });

    /*
    var gui = new dat.GUI();
    gui.add(tracker, 'edgesDensity', 0.1, 0.5).step(0.01);
    gui.add(tracker, 'initialScale', 1.0, 10.0).step(0.1);
    gui.add(tracker, 'stepSize', 1, 5).step(0.1);
    */
};