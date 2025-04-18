# Door Intrusion Detection System 🚪🔔  
ENEL351 Final Embedded Systems Project

## Overview  
This project is a hardware-based **door intrusion detection system** built using the STM32F103RB microcontroller. Developed for the ENEL351 Embedded Systems course, the project integrates an **IR break beam sensor**, an **ultrasonic sensor**, and a **PWM-controlled buzzer**, combined with LEDs and serial communication to provide a functional and responsive security monitoring setup.

The goal was to design a smart, real-time alert system that activates both visual and audible alarms when a door is opened or an object is detected within a certain range. The system also provides real-time data output through USART, enabling external monitoring via a serial terminal like PuTTY.

## Key Features  
- **IR Break Beam Sensor** to detect door open/close events  
- **Ultrasonic Sensor** for distance measurement and object detection  
- **PWM-controlled Buzzer** and **LEDs** for immediate audible and visual feedback  
- **Push Button Reset** to manually silence the alarm  
- **USART Communication** for real-time data display and debugging  
- Modular design with individually testable components  

## System Components  
- **STM32F103RB** Microcontroller (Nucleo Board)  
- **IR Break Beam Sensor** (SEN0503)  
- **Ultrasonic Sensor** (HC-SR04)  
- **Magnetic Buzzer** (CMI-1275-05TH)  
- **LEDs** (IR and Ultrasonic status indicators)  
- **Manual Reset Push Button**  
- **Voltage Divider Circuit** for safe echo signal input  
- **PuTTY Serial Terminal** for data observation

## What I Learned  
This project was a major stepping stone in applying embedded systems principles to a real-world scenario. Some of the key lessons learned include:

- The importance of **GPIO configuration** and voltage level compatibility between components  
- Using **TIM1 and TIM2** for PWM and signal edge detection  
- Implementing real-time systems that react quickly and reliably to hardware events  
- Leveraging **USART debugging** as a powerful tool in embedded development  
- Understanding the iterative process of **hardware-software integration** and debugging

## Demo  
📽️ A video demonstration of the Door Intrusion Detection System will be available here:  
[Watch Project Demo](https://your-link-here.com)

## Schematic  
You can view the full schematic design as a PDF here:  
📄 [View Final Schematic](schematics/ENEL351_Final_Schematic.pdf)  

## Future Improvements  
- Integrate an **OLED display** for visual output  
- Move to **interrupt-based event handling** for better performance  
- Add **wireless communication** (Bluetooth/WiFi) for smart home integration  
- Use **DMA** to improve USART communication efficiency  