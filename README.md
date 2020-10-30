# ESP8266-WiFi-PWM
A simple sketch to control whatever you want from your phone or PC using PWM on the ESP8266.

In my case, I use it to control my room lights with the help of a mosfet to handle the demand for power, but really you can use it for anything that requires you to remotely control PWM.

One thing to keep in mind is, that the esp8266 uses a 10 bit processor, so the PWM scale is not as usual 255, but instead it is 1023.
