(function() {
    window.initPage = function() {
        return $.ajax('./results.json', {
            type: 'GET',
            dataType: 'text',
            success: function(data, textStatus, jqXHR) {
                alert('goldn!');
                var parsed = JSON.parse(data);
            },
            failure: function(data, textStatus, jqXHR) {
                return alert("fail");
            }
        });
    };
}).call(this);
