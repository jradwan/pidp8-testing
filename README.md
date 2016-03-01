# pidp8-testing
PiDP-8 testing &amp; utilities

Just some quick utilities I threw together to test the LEDs on my PiDP-8 kit. I used Paul Bernard's <a href="http://downspout.ca/pidp-test.zip">pidp-test</a> utility as my starting point for learning how to control the PiDP-8's LEDs via GPIO in C (see the <a href="https://groups.google.com/forum/#!searchin/pidp-8/pidp-test/pidp-8/UmIaBv2L9Ts/As6UGcNjCQAJ">Google Group thread</a> for more information). All of the programs rely on the include libraries in the root directory of the project (ledcontrol, ledlocations, pdpcontrol, strcontrol, and gpio).

**HELLO**

A simple "Hello World!" program that can be used as a basic LED test. It loops through turning on ALL of the LEDs and then a simple box animation.

**LEDS-OFF**

Sometimes during my testing I've had random LEDs get stuck on. This simple program turns off ALL of the LEDs.

**MATRIX**

A menu-driven LED test program which is what I spent most of my coding time on once I had learned the basics by writing the two programs above. It includes options to turn all of the LEDs on and off at once, single step through each LED in sequence, animate a chase sequence, and allow for a user-defined animation path. 
