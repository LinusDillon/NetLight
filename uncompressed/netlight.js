(function () {

'use strict';

    var colorHex = {};
    var lightEnabledButton = document.getElementById("lightEnabled");
    // var colorPicker1Hex;
    // var colorPicker2Hex;
    // var colorPicker1 = new ColorPicker(document.getElementById('anim0_color1'), function (hex) {
    //     colorPicker1Hex = hex;
    // });
    // var colorPicker2 = new ColorPicker(document.getElementById('anim0_color2'), function (hex) {
    //     colorPicker2Hex = hex;
    // });

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
        csv += "," + document.getElementById("lightLevel").value;
        return csv;
    }

    function animStatePrefix(i) {
        return "anim" + i;
    }

    function getUiAnimStateAsCsv(i) {
        var prefix = animStatePrefix(i);
        var csv = "" + i;
        csv += "," + colorHex[prefix + "_color1"];
        csv += "," + colorHex[prefix + "_color2"];;
        csv += "," + document.getElementById("anim0_time1_min").value;
        csv += "," + document.getElementById("anim0_time1_max").value;
        csv += "," + document.getElementById("anim0_time2_min").value;
        csv += "," + document.getElementById("anim0_time2_max").value;
        csv += "," + document.getElementById("anim0_effect").value;
        return csv;
    }

    function applyStateToUi(csv) {
        var stateValues = csv.split(",");
        setUiButtonState(lightEnabledButton, parseInt(stateValues[1], 10));
        document.getElementById("lightLevel").value = stateValues[2];
    }

    function applyAnimStateToUi(csv) {
        var anims = csv.split("\n");
        var animValues = anims[0].split(",");
        colorPicker1.setHex(animValues[1]);
        colorPicker2.setHex(animValues[2]);
        document.getElementById("anim0_time1_min").value = animValues[3];
        document.getElementById("anim0_time1_max").value = animValues[4];
        document.getElementById("anim0_time2_min").value = animValues[5];
        document.getElementById("anim0_time2_max").value = animValues[6];
        document.getElementById("anim0_effect").value = animValues[7];
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

    function handleAnimStatusResponse() {
        if (this.status >= 200 && this.status < 400) {
            console.log("RESPONSE: " + this.responseText);
            applyAnimStateToUi(this.responseText);
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
        var csv = getUiStateAsCsv();
        var request = new XMLHttpRequest();
        request.open('PUT', 'status', true);
        request.setRequestHeader('Content-Type', 'text/csv');
        request.onload = handleStatusResponse;
        request.onerror = handleError;
        console.log("REQUEST: " + csv);
        request.send(csv);
    }

    function getAnimState() {
        var request = new XMLHttpRequest();
        request.open('GET', 'anim');
        request.onload = handleAnimStatusResponse;
        request.onerror = handleError;
        request.send();
    }

    function setAnimState(i) {
        var csv = getUiAnimStateAsCsv(i);
        var request = new XMLHttpRequest();
        request.open('PUT', 'anim', true);
        request.setRequestHeader('Content-Type', 'text/csv');
        request.onerror = handleError;
        console.log("REQUEST: " + csv);
        request.send(csv);
    }

    function initAnimState(i) {
        var prefix = animStatePrefix(i);
        document.getElementById(prefix + "_apply").addEventListener("click", function () { 
            setAnimState(i);
        });        
        var colorPicker1 = new ColorPicker(document.getElementById(prefix + "_color1"), function (hex) {
            colorHex[prefix + "_color1"] = hex;
        });
        var colorPicker2 = new ColorPicker(document.getElementById(prefix + "_color2"), function (hex) {
            colorHex[prefix + "_color2"] = hex;
        });
        colorHex[prefix + "_color1"] = "#000000";
        colorHex[prefix + "_color2"] = "#000000";
    }

    document.getElementById("lightEnabled").addEventListener("click", function () {
        setUiButtonState(lightEnabledButton, !getUiButtonState(lightEnabledButton));
        setState();
    });
    document.getElementById("lightLevel").addEventListener("change", function () {
        setState();
    });


    initAnimState(0);
    initAnimState(1);

    getState();
    getAnimState();
    getInfo();

}());