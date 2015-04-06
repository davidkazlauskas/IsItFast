(function() {
    window.initPage = function() {
        return $.ajax('/misc/bench-json', {
            type: 'GET',
            dataType: 'json',
            success: function(data, textStatus, jqXHR) {
                paintAvgMilis(data);
                paintTotHit(data);
                return paintTotTime(data);
            },
            failure: function(data, textStatus, jqXHR) {
                return alert("fail");
            }
        });
    };
}).call(this);
