# Greenhouse Plant Monitoring System using Bluetooth Mesh

**High-Level Overview of the Project:**

This project report focuses on a prototype built for an automated and remote monitoring enabled system for a greenhouse plant that uses Bluetooth Mesh. The project is built on the several concepts learnt throughout the ECEN 5823 course that revolve around building connected IoT devices with varying capabilities and resource constraints while minimizing human interactions at the same time. The project specifically exercises the concepts that were exposed in class on low-energy firmware design and enhancement of IoT systems that provide services on a strict energy budget and constrained deployment.

Greenhouse plants require continuous monitoring and maintenance for its satisfactory and desirable growth which is at the core of the goals within the scope of this project. The project makes use of different sensor units to interact with the environment within the greenhouse to provide care and growth services for the plants contained inside. The sensors connected with the system provide information on the essential environmental properties within the greenhouse like temperature, presence of ambient light, intensity of UV light, and moisture in the greenhouse soil. The connected network architecture offered by Bluetooth Mesh specializes the system with connected nodes that share valuable information – i.e. threshold violations for the set thresholds pertaining to the environmental properties that are being monitored on the different nodes. Upon detection of the violations for any of the set thresholds on any of the nodes will trigger an alarm within the system that notifies the central node – that acts as a bridge in the system for monitoring. Besides threshold violations, the connected nodes will report level readings for the sensor reception.

The project showcases an exhaustive implementation of Bluetooth Mesh network architecture and the concepts associated with its design – i.e. a set of Low Power Nodes and a Friend Node to support the system operations for the greenhouse plant monitoring.

***Please Note:***

The above section has been directly borrowed from the Final Project Report created for this project submission. The work there is duly credited and is utilized here for providing a background reference to the project.

# Final Project Status

•	This project repository provides the firmware that would run on the EFR32 Blue Gecko 13 platform and would provide Bluetooth Mesh Friend Node (Server Model) capabilities.

•	The firmware provides support for handling external events arising from the state machine that runs the MCP9808 temperature sensor that provides temperature readings at an interval of every 1 second.

•	The firmware allows the device to be provisioned using a GATT device (e.g. smartphone) or the embedded provisioner for which the repository link has been provided in this file.

•	The firmware supports simultaneous connections of up to three Low Power Nodes with successful friendship establishment.

•	The firmware has source files that run the EFR32BG13 LETIMER and I2C peripherals for interacting with the external temperature sensor and for also providing timestamps for logging.

•	The firmware includes a GPIO source file that manages all the GPIO requirements – e.g. running the on-board LEDs and providing support for the LCD display.

•	The firmware includes logging support using the log source file.

•	The firmware also logs the error cases for BTM event and gecko APIs/commands run in the application source file.

•	The firmware provides support for Generic Level Server Model as the FN is a subscriber and utilizes the server model.

•	The MCP9808 temperature sensor make use of I2C0 port and operates at both 3.3V/5V power supply. The MCP9808 temperature sensor provides an operational I2C frequency of 400 kHz. The sensor uses its own external event state machine.

•	The firmware provides support for storing persistent data. The firmware essentially uses the Generic Level Server Model to acquire level information from the Low-power Nodes for the sensor that are interfaced with those nodes. Upon threshold violations on these individual LPNs, the FN acquires alarm signals from the LPNs and uses an alarm buffer to set bit flags corresponding to each LPN. The alarm buffer would be then stored in the flash memory to provide persistent data support and at the same time would allow the retention of alarm statuses in case of power cycles on the FN.

•	The firmware on the FN also allows power-saving features by providing the ability to the user to turn off the LCD display and turn it on back when necessary by interacting with the device using PB1 pushbutton.

•	The firmware on the FN also allows a user to override all the alarm signals and refresh the LCD display with no alarm signals. This feature also reflects the new changes in the persistent data (i.e. alarm buffer stored in the flash memory).

•	The firmware also provides support for using Generic On/Off Server Model. However, this is a separate firmware available on the [generic_on_off](https://github.com/CU-ECEN-5823/final-project-assignment-MacRush7/tree/generic_on_off) branch of this repository.

•	The firmware does not provide full DFU support and this feature has been excluded and mentioned as "NOT IMPLEMENTED" in the project validation plan.

# Source Files Included

_List of major source files in the src/main-src/ directory are defined below:_

**cmu.c** - This is the source file for using the Clock Management Unit (CMU) available on the EFR32BG13 platform.

**display.c** - This is an application source file for display support for the on-board LCD available on the EFR32BG13 platform.

**gecko_mesh.c** - This is BTM source file for initializing mesh features on the node.

**gpio.c** - This is the source file for GPIO support.

**i2c.c** - This is the source file for running and utilizing the I2C0 peripheral available on the EFR32BG13 platform.

**letimer.c** - This is the source file for running and utilizing the Low-energy timer available on the EFR32BG13 platform for various applications.

**log.c** - This is the application source file for logging support.

**main_app.c** - This is the main source file application code for initializing the gecko board and handling the external event state machine written for the MCP9808 temperature sensor.

**mcp9808.c** - This is the source file for utilizing the external I2C sensor (MCP9808) interfaced with the EFR32BG13 platform.

**pushbutton.c** - This is the source file for supporting push button interrupt handling and generating external signals in the BTM stack events.

**state.c** - This is the source file that contains the entire state machine written for running the MCP9808 temperature sensor.

_List of major source files in the main directory are defined below:_

**app.c** - This is the source file that runs the BTM stack event handler and incorporates use of other sub-level application source files containing parts of the firmware - **app_src.c** and **app_config.c**.

# Project Documentation References

Link to the root of the shared folder containing all the documents: [Shared Folder](https://drive.google.com/drive/folders/1F-_n8RPkxCzO7q5ui7oLMy1UNxIx9kCD)

Link to my individual folder containing all the documents         : [Individual Folder](https://drive.google.com/drive/folders/1DK5CKNGkKNg4ECu0hMcLmtSz1u8stpVj)

Link to repository containing provisioner source code             : [Mesh Provisioner](https://github.com/CU-ECEN-5823/mesh-provisioner-MacRush7)

# Team Members

[Rushi James Macwan](https://github.com/CU-ECEN-5823/final-project-assignment-MacRush7)

[Bryan Cisneros](https://github.com/CU-ECEN-5823/final-project-assignment-bcis93)

[Atharva Nandanwar](https://github.com/CU-ECEN-5823/final-project-assignment-atharvanan1)

[Abhijeet Dutt Srivastava](https://github.com/CU-ECEN-5823/final-project-assignment-KaiserS0ze)

