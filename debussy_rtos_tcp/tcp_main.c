/*
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *    ======== tcp_main.c ========
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>

#include<xdc/runtime/Timestamp.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

 /* NDK Header files */
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/_stack.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* Example/Board Header files */
#include "Board.h"

#include <stdlib.h>

#define TCPPACKETSIZE 1024 //1024
#define TCPPORT 1000
#define NUMTCPWORKERS 3


/*
 * ========= process recvd command=====
 *  Task to handle the recieved command
 */

Void
probe_pck_cmd(char *data, SOCKET clientfd)
{


   //static unsigned char toggle_LED6 = 0;
   //static unsigned char toggle_LED7 = 0;

   switch(data[0])
   {
		case '1'  :

			//if its ON, LED6 or LED7 are set to be OFF, toggling the LEDs
			//toggle_LED6 = ~toggle_LED6;

	    	//write to the GPIO, Toggle LED6
		    GPIO_write(Board_LED0, Board_LED_OFF);
		    GPIO_write(Board_LED1, Board_LED_ON);
	     	break;

	    case '2'  :
	        //toggle_LED7 = ~toggle_LED7;

	        //write to the GPIO, Toggle LED7
	    	 GPIO_write(Board_LED0, Board_LED_ON);
	         GPIO_write(Board_LED1, Board_LED_OFF);
	        break;

	   case 'X'  :
		   fdClose(clientfd);

	        //turn off the signal after disconnecting, LEDS are ON when there is no signal.
		    GPIO_write(Board_LED0, Board_LED_ON);
		    GPIO_write(Board_LED1, Board_LED_ON);
	        break;

	   default :
		    GPIO_write(Board_LED0, Board_LED_ON);
		    GPIO_write(Board_LED1, Board_LED_ON);

	   }

}
/*
 *  ======== tcpWorker ========
 *  Task to handle TCP connection. Can be multiple Tasks running
 *  this function.
 */
Void tcpWorker(UArg arg0, UArg arg1)
{
    SOCKET clientfd = (SOCKET)arg0;
    Int nbytes;
    Bool flag = TRUE;
    Char *buffer;
    Error_Block eb;

    fdOpenSession(TaskSelf());

    System_printf("tcpWorker: start clientfd = 0x%x\n", clientfd);

    /* Make sure Error_Block is initialized */
    Error_init(&eb);

    /* Get a buffer to receive incoming packets. Use the default heap. */
    buffer = Memory_alloc(NULL, TCPPACKETSIZE, 0, &eb);
    if (buffer == NULL) {
        System_printf("tcpWorker: failed to alloc memory\n");
        Task_exit();
    }

    /* Loop while we receive data */
    while (flag) {
        nbytes = recv(clientfd, (char *)buffer, TCPPACKETSIZE, 0);
        if (nbytes > 0) {
            /* Echo the data back */
            send(clientfd, (char *)buffer, nbytes, 0 );

            probe_pck_cmd((char *)buffer,clientfd);
        }
        else {
            fdClose(clientfd);
            flag = FALSE;
        }
    }
    System_printf("tcpWorker stop clientfd = 0x%x\n", clientfd);

    /* Free the buffer back to the heap */
    Memory_free(NULL, buffer, TCPPACKETSIZE);

    fdCloseSession(TaskSelf());
    /*
     *  Since deleteTerminatedTasks is set in the cfg file,
     *  the Task will be deleted when the idle task runs.
     */
    Task_exit();
}

/*
 *  ======== tcpHandler ========
 *  Creates new Task to handle new TCP connections.
 */
Void tcpHandler(UArg arg0, UArg arg1)
{
    SOCKET lSocket;
    struct sockaddr_in sLocalAddr;
    SOCKET clientfd;
    struct sockaddr_in client_addr;
    Int addrlen=sizeof(client_addr);
    Int optval;
    Int optlen = sizeof(optval);
    Int status;
    Task_Handle taskHandle;
    Task_Params taskParams;
    Error_Block eb;

    fdOpenSession(TaskSelf());

    lSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (lSocket < 0) {
        System_printf("tcpHandler: socket failed\n");
        Task_exit();
        return;
    }

    memset((char *)&sLocalAddr, 0, sizeof(sLocalAddr));
    sLocalAddr.sin_family = AF_INET;
    sLocalAddr.sin_len = sizeof(sLocalAddr);
    sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sLocalAddr.sin_port = htons(arg0);

    status = bind(lSocket, (struct sockaddr *)&sLocalAddr, sizeof(sLocalAddr));
    if (status < 0) {
        System_printf("tcpHandler: bind failed\n");
        fdClose(lSocket);
        Task_exit();
        return;
    }

    if (listen(lSocket, NUMTCPWORKERS) != 0){
        System_printf("tcpHandler: listen failed\n");
        fdClose(lSocket);
        Task_exit();
        return;
    }

    if (setsockopt(lSocket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
        System_printf("tcpHandler: setsockopt failed\n");
        fdClose(lSocket);
        Task_exit();
        return;
    }

    while (TRUE) {
        /* Wait for incoming request */
        clientfd = accept(lSocket, (struct sockaddr*)&client_addr, &addrlen);
        System_printf("tcpHandler: Creating thread clientfd = %d\n", clientfd);

        /* Init the Error_Block */
        Error_init(&eb);

        /* Initialize the defaults and set the parameters. */
        Task_Params_init(&taskParams);
        taskParams.arg0 = (UArg)clientfd;
        taskParams.stackSize = 1024;
        taskHandle = Task_create((Task_FuncPtr)tcpWorker, &taskParams, &eb);
        if (taskHandle == NULL) {
            System_printf("tcpHandler: Failed to create new Task\n");
        }
    }
}

/*
 *  ======== main ========
 */
Int main(Void)
{
    //execution timer
	UInt32 t0,t1,time;

	t0=Timestamp_get32();

	Task_Handle taskHandle;
    Task_Params taskParams;
    Error_Block eb;

    /* Call board init functions */
    Board_initGeneral();
	Board_initGPIO();
    Board_initEMAC();

    System_printf("Starting the TCP DEBUSSY\nSystem provider is set to "
                  "SysMin. Halt the target and use ROV to view output.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /*
     *  Initialize the LEDs on start to be ON, when they are on there is no voltage output for the motor
     *
     * */
    GPIO_write(Board_LED0, Board_LED_ON);
    GPIO_write(Board_LED1, Board_LED_ON);

    /*
     *  Create the Task that farms out incoming TCP connections.
     *  arg0 will be the port that this task listens to.
     */
    Task_Params_init(&taskParams);
    Error_init(&eb);

    taskParams.stackSize = 1024;
    taskParams.priority = 1;
    taskParams.arg0 = TCPPORT;
    taskHandle = Task_create((Task_FuncPtr)tcpHandler, &taskParams, &eb);
    if (taskHandle == NULL) {
        System_printf("main: Failed to create tcpHandler Task\n");
    }

    /* Start BIOS */
    BIOS_start();

    t1=Timestamp_get32();

    time=t1-t0;


    return (0);
}
