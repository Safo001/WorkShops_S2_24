# CAD for UAVs
Welcome to our second workshop, CAD for UAVs. If you’ve attended our Sumobots workshops, you should probably be quite familiar with Fusion 360 by now. If not, feel free to reference our previous workshops- they’re still available on the GitHub.
(https://ubroboticsworkshop.github.io/WorkShops/1_cad/ will cover how to import parts and create new parts to hold them together)

A UAV will experience loads far greater than a Sumobot, with much more reason to cut down weight. In order to have your drone approved for flight, you’ll have to run a static loading simulation and pass an inspection.
Unlike in Sumobots, where the focus was teaching basic use of CAD tools, the focus of this workshop will be to show you the tools and techniques that will allow you to iterate and improve your design.

# Placing components
Below is an image showing how you may choose to lay out your components:

<img width="100%" alt="Screenshot 2023-10-18 at 13 28 03" src="https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/blob/main/Images/DroneParts.png">

A few things to consider:
Keep the antenna away from any wires carrying high current.
Make sure the LiDAR can see the floor.
Give ample room between the propellers and the battery (at least 45mm for 75mm propellers).
Make sure you can program using the USB-C port, leave enough room around the connector for thick cables.
Make sure you can connect the Air Unit and the motor driver board.
Don’t move the accelerometer too far away from the centre of mass.
Put a cover over the air unit so that it won’t be damaged in a crash.
The circular capacitor on the motor driver can be moved but must be kept close to the motor driver.
Make heavy use of the project tool to add mounting holes.
The Air Unit will be mounted with grommets that accept 2mm screws.
Designing a Frame
Files for the main components- Air unit, motor, battery and ESC- are provided.
-	Remember to add some size on your motor mounting holes- the 2mm motor mounting holes should be at least 2.4mm if it’s 3D printed. Also, the motor’s spindle protrudes out of the base so make sure to leave a hole for it- 6mm should be enough.
-	The propeller’s diameter is 75mm, but you’ll want to leave ample room between them and the frame.
-	I have given an OBJ file of the demo drone. This Drone is overbuilt and could be a lot lighter if optimized properly; this model can work as a reference for a functioning drone though.
Forces on a drone
The drone moves by rotating to point in the direction that it intends to accelerate in. In order to rotate, and resist external forces, the drone changes how fast each motor is spinning. We’ll look into this process and how to control this in a future workshop, but for now we just need to consider how these corrections might put stress on the frame.
State	Motor signal	Image
Hover	All motors provide the same thrust	 
Rolling	Motors on the left or right increase speed	 
Pitching	Motors at the front or back increase speed	 
Yawing	To turn the aircraft, diagonal motors produce more thrust. Since these motors are both spinning in the same direction, the drone will turn.	 
The firmware for the flight controller limits how much throttle can be used to make these corrections, so the slower motors will not actually turn off during flight.
Setting up a Static Loading Study
1.	Select the environment tab, and switch to Simulation:
 
2.	Press Simplify and remove any bodies that you don’t want to be involved in the simulation.
 
 
3.	Make sure that the material is set to a plastic. ABS will be a suitable stand-in, having slightly lower tensile strength than PLA (30-43 vs 40-52MPa).
 
4.	Use the Loads tab to add structural loads to your frame.
 
5.	Select the point at which the motor will apply force, the mountings. When using a 3040 propeller, the 1806 BLDC can create about 2N of force.
 
6.	Apply loads to all limbs. It may prove useful to experiment with how the UAV reacts to different situations- hovering, maximum acceleration upwards, turning in various axis.
7.	Add structural constraints- the battery mounts should be considered the anchor point, since the weight of the air unit and the ESC are negligible.
 
 
8.	The Pre-Check should now show a green tick- if it doesn’t, click it for a description as to why.
 
9.	Run the simulation using cloud credits- these cost £1 each, but students get unlimited for free. Make sure not to run this on your personal credit or face financial desolation.
 
10.	Use the results tab to look at the output of the simulation.
 
11.	Look at the output.
 
Results
This looks pretty bad, but the deformation is magnified to allow for easier observation. 
 
The actual deformation can be seen using the buttons in the top left corner- much better! However, this part is very overbuilt for such as small drone. Try to keep the safety factor above 3 to accommodate sudden shocks and create a lighter frame.
If we use the shell tool to hollow out the body instead of using 100% infill, by clicking the body in the explorer:
 
We get the following after re-running the simulation:
 
We can now see how force concentrates at the sharp angles at the base of the arm- this could be easily reduced by adding some fillets, providing further performance gains.

Advanced Tips
Accurately modelling parts produced via FDM is quite difficult, since the material properties can change with chamber temperature, infill type, layer height, print orientation, nozzle temperature, filament moisture content and so on. As such, it is best to take the results of these simulations as a rough guide instead of an accurate performance prediction.
You can modify material settings under the materials tab. This can be used to account for the weaker bonds between layer lines, to check that the stress doesn’t exceed the delamination forces. 
The problem with this is that layer adhesion is determined by many factors, so it can be quite hard to quantify. Overall, it’s best to model your frame to mitigate transmission of force between layer lines.
The process that Fusion 360 uses to hollow out a body is quite different to how it is done in a slicer, so this probably won’t be hugely accurate to recreating the print geometry. 
Event Simulation
A UAV should be designed to not just tolerate in-flight loading, but also dynamic events such as crashes. However, Fusion 360 Event Simulation does not provide accurate results for common FDM materials such as PLA.
