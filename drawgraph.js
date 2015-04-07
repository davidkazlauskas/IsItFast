(function() {
    //var legendTemplate = '<ul class=\"<%=name.toLowerCase()%>-legend\"><% for (var i=0; i<datasets.length; i++){%><li><span style=\"background-color:<%=datasets[i].fillColor%>\"></span><%if(datasets[i].label){%><%=datasets[i].label%><%}%></li><%}%></ul>';
    var legendTemplate = '<ul class=\"<%=name.toLowerCase()%>-legend\"><% for (var i=0; i<datasets.length; i++){%><li><span style=\"background-color:<%=datasets[i].fillColor%>\"><%if(datasets[i].label){%><%}%><%=datasets[i].label%></span></li><%}%></ul>';

    var findData = function(data,sName) {
        var i,len;
        len = data.benchmarks.length;
        for (i = 0; i < len; ++i) {
            if (data.benchmarks[i].name === sName) {
                return data.benchmarks[i];
            }
        }

        return null;
    };

    var findTime = function(data,sName) {
        var i,len;
        len = data.times.length;

        for (i = 0; i < len; ++i) {
            if (data.times[i].name === sName) {
                return data.times[i];
            }
        }

        return null;
    }

    var getFillColor = function(short_name) {
        if (short_name.indexOf("templatious") != 0) {
            return "rgba(0,200,0,0.7)";
        } else {
            return "rgba(100,100,100,0.7)";
        }
    }

    var getHlFillColor = function(short_name) {
        if (short_name.indexOf("templatious") != 0) {
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

    var paintGraph = function(data) {
        var barchartData,moreData,ctx,theData;
        theData = findData(data,"filter-select");
        barchartData = {
            labels: [
                "Filter select benchmark (filter-select)"
            ],
            datasets: [ genDataSet(theData.times[0]) ,
            {
                label: "Templatious version",
                fillColor: "rgba(0,200,0,0.7)",
                strokeColor: "rgba(0,77,0,0.8)",
                highlightFill: "rgba(0,255,0,0.8)",
                highlightStroke: "rgba(0,128,0,0.8)",
                data: [
                    findTime(theData,"templatious_select").time
                ]
            }
            ]
        };
        moreData = {
            responsive: true,
            barValueSpacing: 20,
            barDatasetSpacing: 40,
            legendTemplate: legendTemplate
        };
        ctx = document.getElementById("filter-select-canvas").getContext("2d");
        var chart = new Chart(ctx).Bar(barchartData,moreData);
        window.avgLoad = chart;
        document.getElementById("filter-select-legend").innerHTML = chart.generateLegend();
    };

    window.initPage = function() {
        return $.ajax('./results.json', {
            type: 'GET',
            dataType: 'json',
            success: function(data, textStatus, jqXHR) {
                paintGraph(data);
            },
            failure: function(data, textStatus, jqXHR) {
                return alert("fail");
            }
        });
    };
}).call(this);
