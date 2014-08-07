Netbot-code
===========

teleop code for netbot (not really a network bot anymore, more like a chair-bot)
The Netbot, or ChairBot or whatever its called now, is a 3-wheeled omni-directional robot. each wheel is offset from the other 2 wheels by 120 degrees. because the robot is designed to controlled while riding onboard, the arduino processor must not only do the proper calculations to translate the 2 xy joysticks into the correct wheel speeds to move the robot in teh direction you want it to go, it must also smooth the acceleration and deceleration to avoid throwing you off the robot when you stop.
I'm still working on documentation but much of the code should be self-explanetary 
simplified Kiwi drive inverse kinematics math is from http://www.chiefdelphi.com/media/papers/2390
the smoothing algorithm is just made up, though it is similar to the PID algorithm without the derivitive and a minimal Integral (only increases values by 1 or 0, while proportianal handles anything higher than 1).
