function httpGetAsync(theUrl, callback)
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            callback(xmlHttp.responseText);
    }
    xmlHttp.open("GET", theUrl, true); // true for asynchronous 
    xmlHttp.send(null);
}

let scgCounter = 0;
function SCgContainer(el) {
    // get source
    let viewer = null;
    const source = el.attributes['src'];
    if (!source) {
        el.className = 'invalid';
    } else {
        const id = `scg-content-${scgCounter}`;
        scgCounter++;
        el.setAttribute('id', id);
        el.className = 'scg';
        viewer = new SCgViewer(id);
        httpGetAsync(source.value, function(data) {
            viewer.loadFromData(data);
            viewer.fitSizeToContent();
        });
    }

    return {
        viewer: viewer
    }
}

let views = [];
document.addEventListener('DOMContentLoaded', function() {
    const items = document.querySelectorAll('scg');
    items.forEach(function(d) {
        views.push(new SCgContainer(d));
    });
});