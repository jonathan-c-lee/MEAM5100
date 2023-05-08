const char body[] PROGMEM = R"===(
  <!DOCTYPE html>
  <html>
    <head>
      <style>
        .container {
          height: 100vh;
          width: 100vw;
          display: flex;
          flex-direction: column;
          justify-content: center;
          align-items: center;
        }
        button {
          padding: 0.75rem;
          margin: 0.5rem;
        }
      </style>
      <body>
        <div class="container">
          <div class="row">
            <button type="button" onmousedown="rotate_left()" onmouseup="stop()">Rotate Left</button>
            <button type="button" onmousedown="forwards()" onmouseup="stop()">Forwards</button>
            <button type="button" onmousedown="rotate_right()" onmouseup="stop()">Rotate Right</button>
          </div>
          <div class="row">
            <button type="button" onmousedown="stop()" onmouseup="stop()">Stop</button>
          </div>
          <div class="row">
            <button type="button" onmousedown="left()" onmouseup="stop()">Left</button>
            <button type="button" onmousedown="backwards()" onmouseup="stop()">Backwards</button>
            <button type="button" onmousedown="right()" onmouseup="stop()">Right</button>
          </div>
        </div>
      </body>
    </head>
    <script>
      function move(string) {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", string, true);
        xhttp.send();
      }

      function rotate_left() {
        move("rleft");
      }

      function forwards() {
        move("forwards");
      }

      function rotate_right() {
        move("rright");
      }
      
      function stop() {
      	move("stop");
      }

      function left() {
        move("left");
      }

      function backwards() {
        move("backwards");
      }

      function right() {
        move("right");
      }
        
      document.addEventListener("keypress", function (event) {
        if (event.key == "w") {
          forwards();
        } else if (event.key == "s") {
          backwards();
        } else if (event.key == "a") {
          left();
        } else if (event.key == "d") {
          right();
        } else if (event.key == "q") {
          rotate_left();
        } else if (event.key == "e") {
          rotate_right();
        } else if (event.key == " ") {
          stop();
        }
      });

      document.addEventListener('keyup', stop);
    </script>
  </html>
)===";