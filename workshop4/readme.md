# Assembling and Testing Your Drone
Firstly, make sure that you have all of your parts:

- Air unit
- 2x silver motors
- 2x black motors
- 4x propellers
- Capacitor
- ESC cable
- Air Unit
- 4 * Yellow Grommits
- Battery Cable

<img width="500" src="https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/blob/main/workshop4/Videos/Electronics.jpg">

And the printed parts:

<img width="500" src="https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/blob/main/workshop4/Videos/PrintedParts.jpg">

- Propeller Guards
- Frame
- Battery mount

# Assembly 
Get your Air Unit ready by snaping off the rails, if you haven't already. 

Push 4 grommits into the Air Unit board. This can be a bit difficult, try squishing the grommit to squeeze it through the board.

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/7c90dd00-24de-48df-a7dd-f99b9ae6fdc1

Remove the LiDAR cover, a thin orange file. Check carefully if it has already fallen off, otherwise you may scratch the LiDAR.

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/bb20e618-9f44-439a-a1e0-7cc39bd59e51

Fit the ESC connector.

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/d694c8c8-acc0-4bfe-938f-f40fce734783

Place the Air Unit into the cavity on the bottom of the frame, and use four M2 bolts to secure it.

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/95f320fe-4e49-4986-b4f0-f9991f88dea0

Push 4 grommits into the 4 in 1 ESC.

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/dd1053c3-97e8-4211-8c61-10d019f4d6c8

Use 4 M2 bolts to secure the 4 in 1 ESC to the top of the frame.

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/43d65e90-b0f7-4e27-866d-42bc7dd63a49

Plug in the ESC cable

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/b021f05e-f96a-4259-94c4-3d8d9a3c5d0e

Push some bolts through the frame and the guard, so as to align them. Next, screw the motor down onto it.

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/b7a73cb8-1e47-4572-9239-a0372c5feb96


### Important! The motor nut propeller colours need to the in the same positions as in the videos, as they determine spin direction!

Solder on the motor wires. The best way to do this is to use your soldering stand to hold the wire against the side of the pad, such that the springiness of the wire puts a slight amount of pressure on the ESC. Clean your iron on the stand, before applying a very small amount of solder to the iron. This solder isn't to secure the wire, but to allow the liquid solder to better transfer heat than a dry iron. Let the pad heat up before you add more solder, and then hold the iron on the pad until the solder flows across it.

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/3827fa9a-afa0-4dfc-aa05-6e3b26d2da30

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/65c0ad6d-4bae-456d-b40d-aabbf710668b

Make sure to consider the angle you're soldering from. Put the iron on the side of the ESC away from the components, to reduce the risk of burning them.

The second best, but sometimes needed, way to solder on the wires is by first addind solder to the pad and then melting it while pressing the wire into it. Make sure that the insulation of the wire doesn't touch the solder, and be aware that the wire might heat up. This is a slightly questionable technique, but it's the easiest way to get the job done.

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/1eaa0f6d-09d4-4b70-82b8-a107138e61ee

Next, solder the XT30 battery connector and capacitor to the ESC. Trim down the capacitor legs, and strip the insulation back on the battery wires. 

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/044b5c15-248d-48de-9cad-c10328447741

Press the capacitor legs through the holes in the ESC pads, making sure that the polarity is correct (The capacitor will have a stripe to indicate negative, with minus signs on it. There should also be polarity markings on the ESC). Use the second hand to position the wire on the outside of the pad, and solder it down.

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/d0c8200d-6cd6-4b8d-8212-d3770a6cd13c

Once you're sure the soldering is complete, mount the top part. If in doubt about a joint, call us over to check the joint before testing. 

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/15de63a5-3043-421d-8a90-f4572b7fb201

# Testing
To test the power systems, we'll first use the bench supply. The main benefit of this is that a short circuit will cause a current cap rather than an explosion. Set the current to 500mA, to mitigate how much current can flow in this case. 

### Do not run the motors with the propellers connected! 

Upload the ESC testing sketech to the Air Unit, and disconnect the air unit from the computer. The system will now try to spin its motors 2.5 seconds after being connected to power.

Use the male XT30 connector with two leads soldered onto it to connect the bench supply, paying careful attention to the polarity. 

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/5a78b0ef-956f-4ef9-ad18-9214dca81df2

The ESC will emit a series of beeps, then spin up the motors. Look closely at the screw threads on the motor outputs- They should be screwing downwards, like in this video:

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/20403882/88b82665-31b5-4852-8465-4818063c72b0


If a motor is spinning backwards, it can be reversed by simply swapping two of the motor's wires. On a BLDC motor such as this, it doesn't matter which pair of wires are swapped- swapping any two will reverse it. You can use the technique described earlier of heating a solder blob and then inserting the wire to do this quickly.
