var token, deviceID, eventPrefix
var eventStr;

$.getJSON("config.json", function(json) {
    console.log(json); // this will show the info it in firebug console
    token = json["access_token"];
    deviceID = json["deviceID"];
    eventPrefix = json["eventPrefix"];

    pumpStatus();
});

function getPressure() {
  variable = "pressure";
  var requestURL = "https://api.spark.io/v1/devices/" + deviceID + "/" + variable + "?access_token=" + token;
  $.get( requestURL,
          {},
          function(data) {
              /*if (data.return_value==1) {
                  $("#result").text("Successfully switched pump").fadeTo(1000,1);
                  $("#result").delay(1000).fadeTo(500, 0,
                  function () {
                      $(this).delay(500);
                      $(this).html('');
                      $(this).fadeTo(500, 1);
                  });
              } else {
                  $("#result").text("Failed to switch pump").fadeTo(500,1);
                  $("#result").delay(1000).fadeTo(500, 0,
                  function () {
                      $(this).delay(500);
                      $(this).html('');
                      $(this).fadeTo(500, 1);
                  });
              }*/
              console.log(data);
              $("#pressure").text("Pressure: " + data.result);
          });

  /*var requestURL = "https://api.spark.io/v1/devices/" + deviceID + "/" + variable + "?access_token=" + token;
  $.post( requestURL,
          {},
          function(data) {
              /*if (data.return_value==1) {
                  $("#result").text("Successfully switched pump").fadeTo(1000,1);
                  $("#result").delay(1000).fadeTo(500, 0,
                  function () {
                      $(this).delay(500);
                      $(this).html('');
                      $(this).fadeTo(500, 1);
                  });
              } else {
                  $("#result").text("Failed to switch pump").fadeTo(500,1);
                  $("#result").delay(1000).fadeTo(500, 0,
                  function () {
                      $(this).delay(500);
                      $(this).html('');
                      $(this).fadeTo(500, 1);
                  });
              }
              console.log(data);
          });*/
}

function switchPump() {
  if (document.getElementById("pumpStatus").checked == true) {
    eventPre = eventPrefix + "pumpOn";
  }
  else {
    eventPre = eventPrefix + "pumpOff";
  }

  minsOn = $('#timeOn').find(":selected").text();
  console.log("mins on: " + minsOn);

  var requestURL = "https://api.spark.io/v1/devices/events";
  $.post( requestURL,
          { name: eventPre, data: minsOn, access_token: token },
          function(data) {
              console.log(data);
          });
}
$(function() {
$('#pumpStatus').change(function() {
  console.log("changed");
  switchPump();
});
});

function fillTank(minsOn) {
  eventStr = minsOn;
  var requestURL = "https://api.spark.io/v1/devices/events";
  $.post( requestURL,
          { name: eventPrefix, data: minsOn, access_token: token },
          function(data) {
              console.log(data);
          });
}

function pumpOff() {
  eventStr = 'turn off pump';
  var requestURL = "https://api.spark.io/v1/devices/events";
  $.post( requestURL,
          { name: eventPrefix, data: eventStr, access_token: token },
          function(data) {
              console.log(data);
          });
}

function pumpStatus() {
  variable = "pumpStatus";
  var requestURL = "https://api.spark.io/v1/devices/" + deviceID + "/" + variable + "?access_token=" + token;
  $.get( requestURL,
          {},
          function(data) {
              if (data.result == "on" && document.getElementById("pumpStatus").checked == false) {
                $('#pumpStatus').bootstrapToggle('on');
              } else if (document.getElementById("pumpStatus").checked == true) {
                $('#pumpStatus').bootstrapToggle('off');
              }
          });
}

function minsOnLeft() {
  if (document.getElementById("pumpStatus").checked == true) {
    variable = "minsOnLeft";
    var requestURL = "https://api.spark.io/v1/devices/" + deviceID + "/" + variable + "?access_token=" + token;
    $.get( requestURL,
            {},
            function(data) {
                $('#minsLeft').text("Pump shutting off in " + data.result);
            });
  }
}

var interval = setInterval(pumpStatus, 2000);
