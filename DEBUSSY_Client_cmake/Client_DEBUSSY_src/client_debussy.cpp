/*
* Copyright (c) 2014 Italian Institute of Technology, Center for Micro-Bio Robotics@SSSA.
* All rights reserved.
*
* 
* Client for the DEBUSSY flexible probe. This class handles all network communication with the embedded server and the UI interaction. 
* Due to the simplicity of the current requirements it was not necessary to implement the TCP protocol handlers into separate threat of the UI. Future requirements might have this necessety
* The application connects to the embedded F28M36 TCP server and sends breaking commands to the Server, if the send command is:
*	1. "1" we toggle the breaking system of the Robot1 of the probe
*	2. "2" we toggle the breaking system of the Robot2 of the probe
*  Further development:
*	1. Expand the range of commands for the probe movement
*   2. Network protocol is implemented using Qt networking library. Think if its necessary to expand it to: OpenIgtLink
*
*
*  Author: Risto Kojcev
*  email:  rkojcev@gmail.com
*
*/


#include "client_debussy.h"

Client_DEBUSSY::Client_DEBUSSY(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//connect/disconnect to/from Server button clicks
	connect(ui.btn_Connect,SIGNAL(clicked()),this,SLOT(connectToServer()));
	connect(ui.btn_Disconnect,SIGNAL(clicked()),this,SLOT(closeConnection()));

    //signal and slots for data exchange for the breaking system
	connect(ui.btn_R1_Toggle,SIGNAL(clicked()),this,SLOT(toggle_R1_break()));
	connect(ui.btn_R2_Toggle,SIGNAL(clicked()),this,SLOT(toggle_R2_break()));

	//handlers for the Connections
	connect(&tcpSocket,SIGNAL(connected()),this,SLOT(updateStatus()));
	connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(readyToRead()));
	connect(&tcpSocket,SIGNAL(disconnected()),this,SLOT(connectionClosedByServer()));
	connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error()));

	connect(ui.btn_Quit,SIGNAL(clicked()),this,SLOT(exit()));

}

Client_DEBUSSY::~Client_DEBUSSY()
{
 
}

void Client_DEBUSSY::connectToServer(){

	//static IP of the embedded Server
	QString hostAddress = "192.168.84.162";

	//Open port of the embedded Server
	unsigned short port = 1000;


    //connect to embedded Server
	tcpSocket.connectToHost(hostAddress,port, QIODevice::ReadWrite);
	
	QPalette status_palette;
	status_palette.setColor(QPalette::WindowText, Qt::darkGreen);

	ui.lbl_Status->setAutoFillBackground(true);
	ui.lbl_Status->setPalette(status_palette);

	ui.lbl_Status->setText(tr("Connecting to server..."));
	nextBlockSize=0;


}
/* Handle the key press events
 * In this function we toggle the leds depending on which key is pressed. 
 * Just an addition to the button press user interaction
 *
 */
void Client_DEBUSSY::keyPressEvent(QKeyEvent *event){

	

	QPalette status_palette;
	status_palette.setColor(QPalette::WindowText, Qt::darkGreen);

	ui.lbl_Status->setAutoFillBackground(true);
	ui.lbl_Status->setPalette(status_palette);

	//Check if we are connected before enablling the user to interact using the keyboard
	if (tcpSocket.waitForConnected(1000)){

		if((event->key()== Qt::Key_Q) || (event->key()== Qt::Key_1)){
			toggle_R1_break();
	
		}
		else if((event->key()== Qt::Key_W) || (event->key()== Qt::Key_2)){
			toggle_R2_break();
	
	
		}
		else if((event->key()== Qt::Key_S)){
			closeConnection();

		}
		else{
			QWidget::keyPressEvent(event);
		}
	}
	else{

		QPalette status_palette;
		status_palette.setColor(QPalette::WindowText, Qt::darkRed);

		ui.lbl_Status->setAutoFillBackground(true);
		ui.lbl_Status->setPalette(status_palette);

		ui.lbl_Status->setText(tr("Please connect to the Server first!!!"));


	}

}
void Client_DEBUSSY::toggle_R1_break(){

	QByteArray break_Robot_nr;

	//break_Robot_nr=QByteArray::number(1,10);

	break_Robot_nr="1";

	qDebug() <<"Break_Robot_nr: " <<break_Robot_nr;

	tcpSocket.write(break_Robot_nr + "\n");

	QPalette status_palette;
	status_palette.setColor(QPalette::WindowText, Qt::darkGreen);

	ui.lbl_Status->setAutoFillBackground(true);
	ui.lbl_Status->setPalette(status_palette);

	ui.lbl_Status->setText(tr("Toggle R1 Break "));

}

void Client_DEBUSSY::toggle_R2_break(){


	QByteArray break_Robot_nr;

	//break_Robot_nr=QByteArray::number(2,10);
	break_Robot_nr="2";

	qDebug() <<"Break_Robot_nr: " <<break_Robot_nr;

	tcpSocket.write(break_Robot_nr + "\n");

    QPalette status_palette;
	status_palette.setColor(QPalette::WindowText, Qt::darkGreen);

	ui.lbl_Status->setAutoFillBackground(true);
	ui.lbl_Status->setPalette(status_palette);

	ui.lbl_Status->setText(tr("Toggle R2 Break"));
}

void Client_DEBUSSY::updateStatus(){

	ui.lbl_Status->setText(tr("Connected to Server."));

    ui.btn_R1_Toggle->setEnabled(true);
	ui.btn_R2_Toggle->setEnabled(true);

}

QByteArray Client_DEBUSSY::displayBreakStatus(QByteArray data){

	QByteArray display_str="Break1:OFF\n Break2:OFF";

	QByteArray break1_info="OFF";
	QByteArray break2_info="OFF";

	break1_info=strcmp(data,"1\n")==0 ? "ON":"OFF";
	break2_info=strcmp(data,"2\n")==0 ? "ON":"OFF";
	//if(strcmp(data,"1\n")==0){
	//
	//	break1_info=break1_info ?"ON":"OFF";
	//	//display_str="Break1: " + break1_info ;

	//}
	//else if (strcmp(data,"2")==0 ? ""){

	//	break2_info=!break2_info;
	//
	//}
	//else{
	//
	//}
	return display_str="Break1:<font color=\"red\">"+ break1_info + "</font><br> Break2:<font color=\"red\">" + break2_info + "</font>"; //+ <font color=\"red\"> break1_info + "\n" </font>  + "Break2": + break2_info

}

void Client_DEBUSSY::readyToRead(){

	QByteArray data=tcpSocket.readAll();
	QByteArray info_breaks=displayBreakStatus(data);
	ui.lbl_recvdPckg->setText(info_breaks);

	qDebug()<<data;


}

void Client_DEBUSSY::connectionClosedByServer(){

	if(nextBlockSize != 0xFFFF){
		
		QPalette status_palette;
		status_palette.setColor(QPalette::WindowText, Qt::darkRed);

		ui.lbl_Status->setAutoFillBackground(true);
		ui.lbl_Status->setPalette(status_palette);
		ui.lbl_Status->setText(tr("Error: Connection Closed by server!!"));
	
	}
		closeConnection();
}


void Client_DEBUSSY::error(){
	ui.lbl_Status->setText(tcpSocket.errorString());
	closeConnection();
}

void Client_DEBUSSY::closeConnection(){

    tcpSocket.write("X\n");

	tcpSocket.disconnectFromHost();
    if (tcpSocket.state() == QAbstractSocket::UnconnectedState ||
        tcpSocket.waitForDisconnected(10))
        qDebug("Disconnected!");

	//tcpSocket.close();

	QPalette status_palette;
	status_palette.setColor(QPalette::WindowText, Qt::darkRed);

	ui.lbl_Status->setAutoFillBackground(true);
	ui.lbl_Status->setPalette(status_palette);
	ui.lbl_Status->setText(tr("Disconnected from server..."));

	ui.btn_R1_Toggle->setEnabled(false);
	ui.btn_R2_Toggle->setEnabled(false);


}

void Client_DEBUSSY::exit(){
	tcpSocket.close();

	QPalette status_palette;
	status_palette.setColor(QPalette::WindowText, Qt::darkRed);

	ui.lbl_Status->setAutoFillBackground(true);
	ui.lbl_Status->setPalette(status_palette);
	ui.lbl_Status->setText(tr("Disconnected from server..."));
	close();
    qApp->quit();

}