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
  if (document.getElementById("pumpCheck").checked == true) {
    eventStr = 'turn on pump';
  }
  else {
    eventStr = 'turn off pump';
  }

  var requestURL = "https://api.spark.io/v1/devices/events";
  $.post( requestURL,
          { name: eventPrefix, data: eventStr, access_token: token },
          function(data) {
              console.log(data);
          });
}

function fillTank(minsOn) {
  eventStr = 'turn on pump';
  var requestURL = "https://api.spark.io/v1/devices/events";
  $.post( requestURL,
          { name: eventPrefix, data: eventStr, access_token: token },
          function(data) {
              console.log(data);
          });
  setTimeout(pumpOff, minsOn*60*1000);
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
              console.log(data);
              $("#pumpStatus").html(data.result);
          });
}

var interval = setInterval(pumpStatus, 2000);
