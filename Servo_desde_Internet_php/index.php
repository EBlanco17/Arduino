<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="utf-8">
    <style>
      html {
          font-family: Arial;
          display: inline-block;
          margin: 0px auto;
          text-align: center;
      }
      
      h1 { font-size: 2.0rem; color:#2980b9;}
      h2 { font-size: 1.25rem; color:#2980b9;}
      
      .buttonON {
        display: inline-block;
        padding: 15px 25px;
        font-size: 24px;
        font-weight: bold;
        cursor: pointer;
        text-align: center;
        text-decoration: none;
        outline: none;
        color: #fff;
        background-color: #4CAF50;
        border: none;
        border-radius: 15px;
        box-shadow: 0 5px #999;
      }
      .buttonON:hover {background-color: #3e8e41}
      .buttonON:active {
        background-color: #3e8e41;
        box-shadow: 0 1px #666;
        transform: translateY(4px);
      }
        
      .buttonOFF {
        display: inline-block;
        padding: 15px 25px;
        font-size: 24px;
        font-weight: bold;
        cursor: pointer;
        text-align: center;
        text-decoration: none;
        outline: none;
        color: #fff;
        background-color: #e74c3c;
        border: none;
        border-radius: 15px;
        box-shadow: 0 5px #999;
      }
      .buttonOFF:hover {background-color: #c0392b}
      .buttonOFF:active {
        background-color: #c0392b;
        box-shadow: 0 1px #666;
        transform: translateY(4px);
      }
      .Servo {
        display: inline-block;
        padding: 15px 25px;
        font-size: 24px;
        font-weight: bold;
        cursor: pointer;
        text-align: center;
        text-decoration: none;
        outline: none;
        color: #fff;
        background-color: #3c75e7;
        border: none;
        border-radius: 15px;
        box-shadow: 0 5px #999;
      }
      .Servo:hover {background-color: #2b49c0}
      .Servo:active {
        background-color: #2b49c0;
        box-shadow: 0 1px #666;
        transform: translateY(4px);
      }
      span{font-size: 1.5rem; color:#5829c4;}
      
    </style>

  <script>
    addEventListener('load',inicio,false);

    function inicio()
    {
      document.getElementById('get').addEventListener('change',cambioServo,false);
    }

    function cambioServo()
    {    
      document.getElementById('put').innerHTML=document.getElementById('get').value;
    }
  </script>
  </head>
  <body>
    <h1>Control LED y Servo Motor en Esp32</h1>
    
    <form action="updateDBLED.php" method="post" id="LED_ON" onsubmit="myFunction()">
      <input type="hidden" name="led" value="1"/>    
    </form>
    
    <form action="updateDBLED.php" method="post" id="LED_OFF">
      <input type="hidden" name="led" value="0"/>
    </form>
    
    <button class="buttonON" name= "subject" type="submit" form="LED_ON" value="SubmitLEDON" >LED ON</button>
    <button class="buttonOFF" name= "subject" type="submit" form="LED_OFF" value="SubmitLEDOFF">LED OFF</button>  
   
    <h2>Seleccione los grados a girar</h2>
   <form action="updateDBServo.php" method="post" id="SERVO" onsubmit="myFunction()">
      <input type="range" name="servo" min="0" max="180" id="get"/>&nbsp&nbsp<span id="put">0</span>
    </form>
    <br>
    <button class="Servo" name= "subject" type="submit" form="SERVO" >GIRAR SERVO</button>
    
  </body>
</html>