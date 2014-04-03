/*
* Copyright (c) 2014 Italian Institute of Technology, Center for Micro-Bio Robotics@SSSA.
* All rights reserved.
*
* 
* Client for the DEBUSSY flexible probe. This is the header definitions for the client_debussy.cpp
*
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


#ifndef CLIENT_DEBUSSY_H
#define CLIENT_DEBUSSY_H

#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QDataStream>


#include "ui_client_debussy.h"

class Client_DEBUSSY : public QMainWindow
{
	Q_OBJECT

public:
	Client_DEBUSSY(QWidget *parent = 0);
	~Client_DEBUSSY();

private slots:
	void connectToServer();
	void toggle_R1_break();
	void toggle_R2_break();
	void updateStatus();
	void connectionClosedByServer();
	void readyToRead();
	void error();
	void exit();
	void closeConnection();

	QByteArray displayBreakStatus(QByteArray data);

private:
	
	QTcpSocket tcpSocket;
	quint16 nextBlockSize;
	void keyPressEvent(QKeyEvent *event);

	Ui::Client_DEBUSSYClass ui;
};

#endif // CLIENT_DEBUSSY_H
