const char body[] PROGMEM = R"===(
  <!DOCTYPE html>
    <html>
      <body>
        <button type="button" onclick="stop()">STOP</button>
        <button type="button" onclick="forwards()">FORWARDS</button>
        <button type="button" onclick="backwards()">BACKWARDS</button>
        <button type="button" onclick="right()">RIGHT</button>
        <button type="button" onclick="left()">LEFT</button>
      </body>
      <script>
      function stop() {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "stop", true);
        xhttp.send();
      }
      function forwards() {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "forwards", true);
        xhttp.send();
      }
      function backwards() {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "backwards", true);
        xhttp.send();
      }
      function right() {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "right", true);
        xhttp.send();
      }
      function left() {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "left", true);
        xhttp.send();
      }
      </script>
    </html>
)===";