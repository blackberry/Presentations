Heartbeat
=========
The Heartbeat sample illustrates how to:
-use some query functions to find the smallest supported resolution (brightness is more important than resolution for this demo)
-wait until a "dark" scene is detected (finger is over lens) before turning on the LED
-monitor the brightness level of the scene and apply a heuristic algorithm to determine heartrate of the finger covering the lens

In the future, I would like to update the code to use an FFT instead of the arbitrary heuristic algorithm I came up with.  Just need a bit of spare time to do so.  Feel free to contribute changes back to this sample if you think you can do better :)  It has been suggested that perhaps monitoring the red channel may provide a better signal than the luminance.

