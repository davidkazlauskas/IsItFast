(function() {
    var paintGraph = function(data) {
        var barchartData;
        alert("inner");
    }

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
