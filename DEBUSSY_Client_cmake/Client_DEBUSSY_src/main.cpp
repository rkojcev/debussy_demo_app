/*
* Copyright (c) 2014 Italian Institute of Technology, Center for Micro-Bio Robotics@SSSA.
* All rights reserved.
*
* 
* Client for the DEBUSSY flexible probe. The main class
* The application connects to the embedded F28M36 TCP server and sends breaking commands to the Server, if the send command is:
*	1. "1" we toggle the breaking system of the Robot1 of the probe
*	2. "2" we toggle the breaking system of the Robot2 of the probe
*  Further development:
*	1. Expand the range of commands for the probe movement
*   2. Network protocol is implemented using Qt networking library. Think if its necessary to expand it to: OpenIgtLink
*
*  Author: Risto Kojcev
*  email:  rkojcev@gmail.com
*
*/


#include "client_debussy.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Client_DEBUSSY w;
	w.show();
	return a.exec();
}
