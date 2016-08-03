(function () {

'use strict';

    var lightEnabledButton = document.getElementById("current_lightEnabled");
    var colorPicker1Hex;
    var colorPicker2Hex;
    var colorPicker1 = new ColorPicker(document.getElementById('current_color1'), function (hex) {
        colorPicker1Hex = hex;
    });
    var colorPicker2 = new ColorPicker(document.getElementById('current_color2'), function (hex) {
        colorPicker2Hex = hex;
    });

    function addClass(el, className) {
        if (el.classList) {
            el.classList.add(className);
        } else {
            el.className += ' ' + className;
        }
    }

    function removeClass(el, className) {
        if (el.classList) {
            el.classList.remove(className);
        } else {
            el.className = el.className.replace(new RegExp('(^|\\b)' + className.split(' ').join('|') + '(\\b|$)', 'gi'), ' ');
        }
    }

    function toggleClass(el, className) {
        if (el.classList) {
            el.classList.toggle(className);
        } else {
            var classes = el.className.split(' ');
            var existingIndex = classes.indexOf(className);

            if (existingIndex >= 0) {
                classes.splice(existingIndex, 1);
            } else {
                classes.push(className);
            }

            el.className = classes.join(' ');
        }
    }

    function setUiButtonState(el, en) {
        if (en) {
            el.innerHTML = "On";
            addClass(el, "button-on");
            addClass(el, "pure-button-active");
        } else {
            el.innerHTML = "Off";
            removeClass(el, "button-on");
            removeClass(el, "pure-button-active");
        }
    }

    function getUiButtonState(el) {
        return el.classList.contains("button-on");
    }


    function applyInfoToUi(csv) {
        var stateValues = csv.split(",");
        document.getElementById("mqtt_server").value = stateValues[1];
        document.getElementById("mqtt_port").value = stateValues[2];
        document.getElementById("mqtt_idx").value = stateValues[3];
        document.getElementById("flashSizeReal").value = stateValues[4];
        document.getElementById("flashSizeCompiler").value = stateValues[5];
        document.getElementById("freeHeap").value = stateValues[6];
        document.getElementById("sketchSize").value = stateValues[7];
        document.getElementById("sketchFree").value = stateValues[8];
    }

    function getUiInfoAsCsv() {
        var csv = "Info";
        csv += "," + document.getElementById("mqtt_server").value;
        csv += "," + document.getElementById("mqtt_port").value;
        csv += "," + document.getElementById("mqtt_idx").value;
        csv += "," + document.getElementById("flashSizeReal").value;
        csv += "," + document.getElementById("flashSizeCompiler").value;
        csv += "," + document.getElementById("freeHeap").value;
        csv += "," + document.getElementById("sketchSize").value;
        csv += "," + document.getElementById("sketchFree").value;
        return csv;
    }

    function getUiStateAsCsv() {
        var csv = "State";
        csv += "," + (getUiButtonState(lightEnabledButton) ? 1 : 0);
        csv += "," + document.getElementById("current_lightLevel").value;
        csv += "," + colorPicker1Hex;
        csv += "," + colorPicker2Hex;
        csv += "," + document.getElementById("current_time1_min").value;
        csv += "," + document.getElementById("current_time1_max").value;
        csv += "," + document.getElementById("current_time2_min").value;
        csv += "," + document.getElementById("current_time2_max").value;
        csv += "," + document.getElementById("current_effect").value;
        return csv;
    }

    function applyStateToUi(csv) {
        var stateValues = csv.split(",");
        setUiButtonState(lightEnabledButton, parseInt(stateValues[1], 10));
        document.getElementById("current_lightLevel").value = stateValues[2];
        colorPicker1.setHex(stateValues[3]);
        colorPicker2.setHex(stateValues[4]);
        document.getElementById("current_time1_min").value = stateValues[5];
        document.getElementById("current_time1_max").value = stateValues[6];
        document.getElementById("current_time2_min").value = stateValues[7];
        document.getElementById("current_time2_max").value = stateValues[8];
        document.getElementById("current_effect").value = stateValues[9];
    }

    function handleInfoResponse() {
        if (this.status >= 200 && this.status < 400) {
            console.log("RESPONSE: " + this.responseText);
            applyInfoToUi(this.responseText);
        } else {
            console.log("Server responded with status: " + this.status);
        }
    }

    function handleStatusResponse() {
        if (this.status >= 200 && this.status < 400) {
            console.log("RESPONSE: " + this.responseText);
            applyStateToUi(this.responseText);
        } else {
            console.log("Server responded with status: " + this.status);
        }
    }

    function handleError() {
        console.log("Connection error.");
    }

    function getInfo() {
        var request = new XMLHttpRequest();
        request.open('GET', 'info');
        request.onload = handleInfoResponse;
        request.onerror = handleError;
        request.send();
    }

    function getState() {
        var request = new XMLHttpRequest();
        request.open('GET', 'status');
        request.onload = handleStatusResponse;
        request.onerror = handleError;
        request.send();
    }

    function setState() {
        var stateCsv = getUiStateAsCsv();
        var request = new XMLHttpRequest();
        request.open('PUT', 'status', true);
        request.setRequestHeader('Content-Type', 'text/csv');
        request.onload = handleStatusResponse;
        request.onerror = handleError;
        console.log("REQUEST: " + stateCsv);
        request.send(stateCsv);
    }

    document.getElementById("current_lightEnabled").addEventListener("click", function () {
        setUiButtonState(lightEnabledButton, !getUiButtonState(lightEnabledButton));
        setState();
    });
    document.getElementById("apply").addEventListener("click", setState);

    getState();
    getInfo();

}());