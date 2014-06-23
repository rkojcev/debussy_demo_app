debussy_demo_app
================

DEBUSSY flexible probe breaking system client app.

This application is the client application of the breaking system for the DEBUSSY flexible probe. 
The application provides TCP/IP connection to a remote embedded server, which runs on embedded RTOS. 
The user through the UI can either enable or disable one of the breaks of the DEBUSSY probe. 
The embedded application takes care of accepting and analyzing the recieved package and generate low level commands, ON/OFF voltage for enabling or disabling the breaking system of the probe.
