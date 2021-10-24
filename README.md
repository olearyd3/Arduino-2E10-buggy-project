# 2E10: Engineering Design IV: Project - Arduino-2E10-buggy-project

This was a semester long project in which a buggy was to be constructed. There were three 'challenges' for this project - bronze, silver and gold.

These are outlined below:

## Bronze Challenge
In summary: Traverse the track, don’t crash into any obstacles, report events wirelessly.

In detail:

Assemble your buggy

Construct a track according to the track specification

Starting and stopping the buggy via wireless control, have it traverse the track twice. Pause for any obstacles, reporting these events to the control PC.
	
Details:

-> The track should consist of a line forming a loop with a total track length (perimeter) of at least 3m.

-> The track itself is a line on a background of contrasting colour (such that it the line can be detected by the IR sensors). A light line on a dark background 		    or vice versa; black marker or electrical tape on a white paper background would be suitable.

-> The track should form a loop (see image below; you do not need to copy this precisely but your track should approximate the shape, and in particular you must 		    include at least two right-angle turns. The loop may be closed with a longer gentle curve if that is easier for you to make). The total length should be 3m.

The PC control program must:

-> Provide the user with start and stop buttons that can be used to begin and end the buggy's run on the track.

-> Provide an output area that displays telemetry received from the buggy during the run. 

The buggy must:

-> Start the run on receiving a GO command via WiFi  from the controlling PC

-> Stop the run on receiving a STOP command via WiFi from the controlling PC

-> Traverse the track twice without derailing, using the IR sensors to follow the line of the track

-> Pause for obstacles as detected by the US rangefinder. The stopping distance is up to you (but about 10cm is reasonable).

-> Report to the controlling PC when obstacles are detected and cleared (a simple "obstacle seen" message is sufficient, but you may choose to do something more details, e.g. "stopping for obstacle at 5cm distance"). The reporting does not have to display within the Processing graphics window (you can use the console).

## Silver Challenge

In addition to the Bronze challenge requirements , in the silver challenge:  

You must use a PID implementation to follow an object smoothly using the ultrasonic sensor.  This means that if a static object is held in front of the buggy then it will stop when it comes close to it (as in Bronze), but if the object is moved along the track then the buggy will adjust it's speed to maintain a comfortable distance from the object.

The buggy must maintain a comfortable safe distance from the object

The buggy must avoid both collisions and avoid lagging too far behind. 

You must display telemetry data from the buggy (obstacle reports and distance reports) within the GUI. 
	
You must lead your buggy around the track 2 times (one clockwise loop, and one anticlockwise loop) using a  combination of the line following im plementation devised for the Bronze challenge and your ultrasonic PID implementation.   

## Gold Challenge

Impress us; get as far as you can and document your work well.  
The one stipulation here is that y ou must  use  the Nano 33 IoT’s on-board  IMU and  make creative  use of the acceleration and/or orientation data.  Consider reporting your heading and plotting it on screen.
