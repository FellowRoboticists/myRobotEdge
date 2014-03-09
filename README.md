myRobotEdge Arduino Sketch
==========================

This sketch teaches the robot to stay within an area delimited by low(er)
reflectance. For example, you can set the robot down on a white piece of poster
board on a surface with less reflectance (like carpet), and the robot will
attempt to stay within the bounds of the poster board.

You will note that some of the code here looks like some of the library code we've
got in the other repositories. They are not the same. The edge sketch is a 
relatively small subset of the actual full libraries.

Programming
===========

Here is how the thing is programmed.

If there is no edge at startup, move forward at minimum speed. 

If while moving forward an edge is detected by both sensors, move backward for
a half-second, then rotate right 120 degrees. Again, move forward.

If while moving forward an edge is detected by the left sensor, move back for
a half-second, then rotate right 60 degrees. Again, move forward.

If while moving forward an edge is detected by the right sensor, move back for
a half-secon, then rotate left 60 degrees. Again, move forward.

Building/Testing
================

Preparation:

1. Determine the directory in which you will place the myRobotEdge project
2. Clone the https://github.com/FellowRoboticists/arduino-tasks project. This provides the necessary ruby tasks use by the rake command.
3. Clone the https://github.com/FellowRoboticists/myRobotEdge project.
4. In the myRobotEdge project directory, invoke the 'rake' command. This will automatically pull all the dependency libraries from GitHub and prepare the ino project for building.

To build the example sketch:

```
ino build
```

To upload the example sketch:

```
ino upload
```

Copyright
=========

Copyright (c) 2012 Michael Margolis

Copyright (c) 2013,2014 Dave Sieh

See LICENSE.txt for details.
