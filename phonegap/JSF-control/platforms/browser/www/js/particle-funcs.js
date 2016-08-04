var token = "3fc0585f05288048cf7f3a16c85fecd9aa7106c6";
var deviceID = "270037000347343337373739";
var eventPrefix = "jsf/waterSystem";
var variable = "pressure";
var eventStr;


function getPressure() {

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
          });
}

var interval = setInterval(getPressure, 2000);
