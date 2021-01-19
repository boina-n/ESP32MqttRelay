<br />
<p align="center">


  <h1 align="center">ESP32MQTTRELAY</h3>

  <p align="justify">
    A simple program using mqtt messages for trigering a relay connected to an ESP32. The program watch on the mqtt broker for a message or command ("u") and uses the relay to actionate (close the circuit) a garage door remote.
  </p>
</p>

## How to configure the project
Everything is configuratble on the config.h file. Simply replace the example values and make sure you have the "PubSubClient" library downloaded in your system library folder (With Arduino IDZ: (Menu) Sketch --> Include Library --> "Add ZIP. Library" 

[PubSubClient](https://github.com/Imroy/pubsubclient) : Download ZIP



#### Pin used to control the relay is 4.

Have Fun!

Nadj