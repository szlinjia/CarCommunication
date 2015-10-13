Problem
===
![](http://debuggingnow.com/wp-content/uploads/2015/10/bridge.png)<br>
Above shows a section of traffic route around the narrow bridge AB on a river. Two red cars(R1,R2) and two blue cars(B1,B2) move along the designated routes that involve indefinite number of trips across the bridge. The bridge is so narrow that at any time, multiple cars cannot pass in opposite directions.<br>
a, Using the Message-passing model, design a decentralized protocol so that at most one car is on the bridge at any time and no car is idefinitely preventd from crossing the bridge. Treat each car to be a process and assume that their clocks ar not synchronized.<br>
b,Modify the protocol so that multiple cars can be on the bridge as long as they are moving in the same direction, but no car is indefinitely prevented from crossing the bridge.<br>
Design a graphical user interface to display the movement of the cars, so that the observer can visualize the cars, control their movements, and verify the protocol.<br>
Solution
===
This program runs in two different platforms. For server part, a logic to act distribute system for Lamport algorithm, is written by C++ and runs in Linux. For client part, the UI to perform car movement, is written by java and runs in Android emulator.<br>
Here is my program video link:<br>
http://youtu.be/_XIKJeisfIs 
How to run
===
There are two folder in submission: Server and Car<br>
#### Compile and run Server<br>
* Step 1:Deploy server folder in ubuntu OS server.<br>
* Step 2:In Server folder, input this command line to compile:<br>
      $ make<br>
      Two executebal files are created: launch and discar<br> 
* Step 3: Then run program：<br>
      $ . /launch 192.168,173.16 ,where IP is the client address to draw graphic.

#### Run client
Client runs in Android emulator. So Android studio and Java needs to be installed.<br>
* Step 1: install java environment; <br>
* Step2: install Android studio;<br>
    http://developer.android.com/sdk/index.html<br>
* Step3:install Android SDK;<br>
    http://developer.android.com/sdk/installing/index.html<br>
* Step 4: import Car folder into Android studio;<br>
* Step5: run program;<br>

Implementation
===
There are two major modules in this system: Discar and UI. The Discar is the core system that controls car logic movement and they are running in different mechines. Each car has four states. Their state transition is showed in following figure<br>
![](http://debuggingnow.com/wp-content/uploads/2015/10/carstatus.png)
#### Sequence Diagram 
![](http://debuggingnow.com/wp-content/uploads/2015/10/carsequence2.jpg)


