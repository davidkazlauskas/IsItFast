(function() {
    var paintGraph = function(data) {
        var barchartData,moreData,ctx;
        barchartData = {
            labels: data.benchmarks.map(function(i) {
                alert(i.name);
                return i.name;
            }),
            datasets: [
            {
                fillColor: "rgba(0,200,0,0.7)",
                strokeColor: "rgba(0,77,0,0.8)",
                highlightFill: "rgba(0,255,0,0.8)",
                highlightStroke: "rgba(0,128,0,0.8)",
                data: data.benchmarks.map(function(i) {
                    return 0.77;
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
