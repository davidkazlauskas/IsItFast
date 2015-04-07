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

    var paintAll = function(data) {
        var graphno,i;
        graphno = 0;

        //for (i = 0; i < data.benchmarks.length; ++i) {
        for (i = 0; i < 1; ++i) {
            paintGraph(data.benchmarks[i]);
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

    var paintGraph = function(data) {
        var barchartData,moreData,ctx,fltData;
        fltData = filterOutPadding(data);
        //alert('alive');
        barchartData = {
            labels: [
                data.full_name + " [" + data.name + "]"
            ],
            //datasets: [ genDataSet(data.times[0]),
            //genDataSet(data.times[1]),
            //genDataSet(data.times[2])
            //]
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
                //paintGraph(data);
                paintAll(data);
            },
            failure: function(data, textStatus, jqXHR) {
                return alert("fail");
            }
        });
    };
}).call(this);
