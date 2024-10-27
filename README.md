# High-fidelity Data Logger

## Project Overview
The High-fidelity Data Logger project is aimed at enhancing data acquisition efficiency using a custom setup based on the Teensyduino microcontroller. This project involved designing a versatile data logging system capable of handling multiple data inputs simultaneously, improving project timelines and data collection capabilities through optimized project management and Agile methodology.

## Key Achievements
- **Increased Data Acquisition Efficiency**: Enhanced data acquisition efficiency by 20% through the use of Teensyduino with Arduino, enabling streamlined data collection from multiple inputs.
- **Project Timeline Optimization**: Reduced overall project completion time by 15% through effective project management strategies and Agile practices.

## System Requirements
- **Microcontroller**: Teensyduino 4.1 with Arduino
- **SD Card**: Compatible with Teensyduino SPI connections
- **Libraries**: SD, SPI, TimeLib, EEPROM
- **Additional Hardware**: LEDs, push button

## Features
- **Multiple Serial Inputs**: Allows logging from two serial sources (Serial1 and Serial3) to separate files.
- **LED Indicators**: Indicate file write operations for visual monitoring.
- **Unique Directory Creation**: Creates a unique directory for each logging session, managed through EEPROM to handle up to 256 sessions.
- **Interrupt-Driven File Retrieval**: Push button enables reading and printing all saved files to the serial monitor.

## Project Management
This project was developed in an Agile environment, ensuring iterative development and efficient resource management, resulting in a substantial reduction in project timeline and enhanced logging efficiency.

## License
This project is open source and can be freely modified and distributed.