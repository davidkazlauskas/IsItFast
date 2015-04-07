(function() {
    //var legendTemplate = '<ul class=\"<%=name.toLowerCase()%>-legend\"><% for (var i=0; i<datasets.length; i++){%><li><span style=\"background-color:<%=datasets[i].fillColor%>\"></span><%if(datasets[i].label){%><%=datasets[i].label%><%}%></li><%}%></ul>';
    var legendTemplate = '<ul class=\"<%=name.toLowerCase()%>-legend\"><% for (var i=0; i<datasets.length; i++){%><li><span style=\"background-color:<%=datasets[i].fillColor%>\"><%if(datasets[i].label){%><%}%><%=datasets[i].label%></span></li><%}%></ul>';

    var findData = function(data,name) {
        var i,len;
        len = data.benchmarks.length;
        for (i = 0; i < len; ++i) {
            if (data.benchmarks[i].name === name) {
                return data.benchmarks[i];
            }
        }

        return null;
    };

    var findTime = function(data,name) {
        var i,len;
        len = data.times.length;

        for (i = 0; i < len; ++i) {
            if (data.times[i].name === name) {
                return data.times[i];
            }
        }

        return null;
    }

    var paintGraph = function(data) {
        var barchartData,moreData,ctx,theData;
        theData = findData(data,"filter-select");
        barchartData = {
            labels: [
                "Filter select benchmark (filter-select)"
            ],
            datasets: [
            {
                label: "Boilerplate version",
                fillColor: "rgba(100,100,100,0.7)",
                strokeColor: "rgba(77,77,77,0.8)",
                highlightFill: "rgba(150,150,150,0.8)",
                highlightStroke: "rgba(128,128,128,0.8)",
                data: [
                    findTime(theData,"BOILERPLATE").time
                ]
            }
            ,
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
