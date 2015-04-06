(function() {
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

    var paintGraph = function(data) {
        var barchartData,moreData,ctx,theData;
        theData = findData(data,"filter-select");
        barchartData = {
            labels: theData.times.map(function(i) {
                return i.full_name;
            }),
            datasets: [{
                fillColor: "rgba(0,200,0,0.7)",
                strokeColor: "rgba(0,77,0,0.8)",
                highlightFill: "rgba(0,255,0,0.8)",
                highlightStroke: "rgba(0,128,0,0.8)",
                data: theData.times.map(function(i) {
                    return i.time;
                })
            }]
        };
        moreData = {
            responsive: true
        };
        ctx = document.getElementById("bench-canvas").getContext("2d");
        window.avgLoad = new Chart(ctx).Bar(barchartData,moreData);
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
