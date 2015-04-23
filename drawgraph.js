(function() {
    var legendTemplate = '<ul class=\"<%=name.toLowerCase()%>-legend\"><% for (var i=0; i<datasets.length; i++){%><li><span style=\"background-color:<%=datasets[i].fillColor%>\">- <%if(datasets[i].label){%><%}%><%=datasets[i].label%></span></li><%}%></ul>';

    var getFillColor = function(short_name) {
        if (short_name.indexOf("templatious") != -1) {
            return "rgba(0,200,0,0.7)";
        } else {
            return "rgba(100,100,100,0.7)";
        }
    }

    var getHlFillColor = function(short_name) {
        if (short_name.indexOf("templatious") != -1) {
            return "rgba(150,200,150,0.8)";
        } else {
            return "rgba(150,150,150,0.8)";
        }
    }

    var genDataSet = function(time) {
        return {
            label: (time.full_name + " [" + time.name + "]"),
            fillColor: getFillColor(time.name),
            strokeColor: "black",
            highlightFill: getHlFillColor(time.name),
            highlightStroke: "black",
            data: [
                time.time
            ]
        };
    }

    var paintAll = function() {
        var graphno,i,data,metadata,genHtml,infoString;

        metadata = getJson('./compiler-info.json');
        data = getJson('./results.json');

        if (!metadata || !data) {
            document.getElementById("the-body").innerHTML =
                "<h1>Run benchmarks at least once to view results.</h1>";
            return;
        }

        infoString = "Compiler: " + metadata.compiler
            + " ; Flags: " + metadata.flags;

        graphno = 0;
        genHtml = '';
        for (i = 0; i < data.benchmarks.length; ++i) {
            genHtml += generateCanvas(i);
            if (i < data.benchmarks.length - 1) {
                genHtml += '<hr/>';
            }
        }
        document.getElementById("the-body").innerHTML = genHtml;

        for (i = 0; i < data.benchmarks.length; ++i) {
            paintGraph(data.benchmarks[i],i);
            document.getElementById('graph-flags-'+i).innerHTML = infoString;
        }
    }

    var filterOutPadding = function(data) {
        return data.times.filter(function(value) {
            if (value.name.indexOf("PADDING") != -1) {
                return false;
            } else {
                return true;
            }
        });
    }

    var getJson = function(file) {
        var result;
        $.ajax(file, {
            type: 'GET',
            dataType: 'json',
            async: false,
            success: function(data,textStatus,jqXHR) {
                result = data;
            }
        });
        return result;
    }

    var generateCanvas = function(id) {
        var result = '';
        result += '<div id="graph-div-';
        result += id;
        result += '" class="barchart">';
        result += '<canvas id="graph-canvas-';
        result += id;
        result += '" width="600" height="450"></canvas>';
        result += '<p class="label" id="graph-label-';
        result += id;
        result += '"></p>';
        result += '<div class="legend" id="graph-legend-';
        result += id;
        result += '"></div>';
        result += '<div class="flags" id="graph-flags-';
        result += id;
        result += '"></div>';
        result += '</div>';
        return result;
    }

    var paintGraph = function(data,idx) {
        var barchartData,moreData,ctx,fltData;
        fltData = filterOutPadding(data);
        barchartData = {
            labels: [
                'microseconds'
            ],
            datasets: fltData.map(function(value) {
                return genDataSet(value);
            })
        };
        moreData = {
            responsive: true,
            barValueSpacing: 20,
            barDatasetSpacing: 40,
            legendTemplate: legendTemplate
        };
        ctx = document.getElementById('graph-canvas-'+idx).getContext("2d");
        var chart = new Chart(ctx).Bar(barchartData,moreData);
        window.avgLoad = chart;
        document.getElementById('graph-legend-'+idx).innerHTML = chart.generateLegend();
        document.getElementById('graph-label-'+idx).innerHTML =
            data.full_name + " [" + data.name + "]";
    };

    window.initPage = function() {
        paintAll();
    };
}).call(this);
