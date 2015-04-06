(function() {
    var paintGraph = function(data) {
        var barchartData;
        barchartData = {
            labels: data.benchmarks.map(function(i) {
                alert(i.name);
                return i.name;
            })
        };
    };

    window.initPage = function() {
        return $.ajax('./results.json', {
            type: 'GET',
            dataType: 'json',
            success: function(data, textStatus, jqXHR) {
                alert(data);
                paintGraph(data);
            },
            failure: function(data, textStatus, jqXHR) {
                return alert("fail");
            }
        });
    };
}).call(this);
