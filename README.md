myRobotEdge Arduino Sketch
==========================

This sketch teaches the robot to stay within an area delimited by low(er)
reflectance. For example, you can set the robot down on a white piece of poster
board on a surface with less reflectance (like carpet), and the robot will
attempt to stay within the bounds of the poster board.

You will note that some of the code here looks like some of the library code we've
got in the other repositories. They are not the same. The edge sketch is a 
relatively small subset of the actual full libraries.

Building/Testing
================

Preparation:

1. Determine the directory in which you will place the Battery project
2. Clone the https://github.com/FellowRoboticists/arduino-tasks project. This provides the necessary ruby tasks use by the rake command.
3. Clone the https://github.com/FellowRoboticists/Battery project.
4. In the Battery project directory, invoke the 'rake' command. This will automatically pull all the dependency libraries from GitHub and prepare the ino project for building.

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
