//============================================================================
//============================================================================
//
// FILE:	SciCommsCRC.c
//
// TITLE:	GP Comms kernel as an interface to external GUI and Check the CRC code
//
// Version1 : 1 Feb 2016 - Release 1 - Internal Release (BRL)
//============================================================================
//============================================================================
#include "PeripheralHeaderIncludes.h"
#include "stdint.h"
#include "inttypes.h"
#include "hdlc.h"
#include "crc.h"

#define CPU_FREQ 	80E6        // Default = 40 MHz. Change to 60E6 for 60 MHz devices
#define LSPCLK_FREQ CPU_FREQ/4
#define SCI_FREQ 	115200
#define SCI_PRD 	(LSPCLK_FREQ/(SCI_FREQ*8))-1

//! Test Packet Length in number of words
#define PACKET_LEN			40
#define R_PACKET_LEN		100
//! Test Packet Length in number of BYTES
#define PACKET_BYTES   (PACKET_LEN << 1)

// Prototype statements for functions found within this file.
__interrupt void sciaTxFifoIsr(void);
__interrupt void sciaRxFifoIsr(void);
void scia_fifo_init(void);
void error(void);
int HDLC_encode(const HDLC_pck *decode_pck, HDLC_pck *encode_pck);



//HDLC STATUS
#define HDLC_BEGIN              0X01    //BEGIN TO RECEIVE DATA
#define HDLC_END                0x02
#define HDLC_ERROR              0x03

int hdlc_status = 0;
int curr_Rx_pos = 0;
int curr_Tx_pos = 0; //sending Tx- curr
HDLC_pck recv, send;  //Initial the Pockage
int hdlc_escape = 0;
crc crc_result;

int error_count = 0;
int success_count = 0;


// Global variables
extern uint16_t sdataA[2];    // Send data for SCI-A
extern uint16_t rdataA[0xFF];    // Received data for SCI-A


HDLC_pck table;

void error(void) {
	__asm("     ESTOP0");
	// Test failed!! Stop!
	for (;;)
		;
}

__interrupt void sciaTxFifoIsr(void) {
	uint16_t i; //every four char generate one time interrupt.
	for (i = 0; i < 4; i++) {
		SciaRegs.SCITXBUF = send.pck[curr_Tx_pos];
		if (send.pck[curr_Tx_pos] == HDLC_END_FLAG) {
			curr_Tx_pos = 0; //RESET curr
			HDLC_encode(&table, &send);
		} else {
			curr_Tx_pos++;
		}
	}

	SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;	// Clear SCI Interrupt flag
	PieCtrlRegs.PIEACK.all |= 0x100;      // Issue PIE ACK
}

__interrupt void sciaRxFifoIsr(void) {
	uint16_t i;			//Declare 4 times circul
	uint8_t sciRxBuf; // Declare the one byte buffer

	// the following code is to decode the CRC
	for (i = 0; i < 4; i++) {
		sciRxBuf = SciaRegs.SCIRXBUF.all; // read one time data
		if (sciRxBuf == HDLC_START_FLAG) {  //start of the packet
			//printf("here\n");
			hdlc_status = HDLC_BEGIN;     //RESET STATUS
			//start timer, reset position
			curr_Rx_pos = 0;
		} else if (sciRxBuf == HDLC_END_FLAG) {       //end of the packet
			if (hdlc_status == HDLC_BEGIN) {      //STATUS CORRECT
				hdlc_status = HDLC_END;
				recv.len = curr_Rx_pos - 2;
				crc_result = crcFast(recv.pck, curr_Rx_pos);
				if (crc_result == 0) {
					//	printf("The received packet len: %d\n", recv.len);
					success_count ++;
				} else {    //error
					hdlc_status = HDLC_ERROR;
					//error();
					error_count++;
					if (error_count>1000){
						error();
					}
				}
			}
		} else        //middle of the packet
		{
			if (hdlc_status == HDLC_BEGIN && curr_Rx_pos < HDLC_PKT_MAXLEN - 1) {
				if (sciRxBuf == HDLC_ESCAPE_BYTE) {
					hdlc_escape = 1;
					i++;
				} else {
					if (hdlc_escape) {
						hdlc_escape = 0;
						recv.pck[curr_Rx_pos++] = sciRxBuf ^ HDLC_XOR_BYTE;
					} else {
						recv.pck[curr_Rx_pos++] = sciRxBuf;
					}
					//                        printf("middle %d: %X\n ",curr_Rx_pos, buf[i]);
				}
			} else {  //error
				hdlc_status = HDLC_ERROR;
			}
		}

		if (hdlc_status == HDLC_ERROR) {
			//printf("receiving data error. \n");
		}
	}
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;   // Clear Overflow flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all |= 0x100;   // Issue PIE ack
}

void scia_fifo_init() {

// This is needed to disable write to EALLOW protected registers
// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
	EALLOW;
// This is needed to write to EALLOW protected registers
	PieVectTable.SCIRXINTA = &sciaRxFifoIsr;
	PieVectTable.SCITXINTA = &sciaTxFifoIsr;
	EDIS;

	SciaRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
									// No parity,8 char bits,
									// async mode, idle-line protocol
	SciaRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
									// Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.bit.TXINTENA = 1;
	SciaRegs.SCICTL2.bit.RXBKINTENA = 1;
	SciaRegs.SCIHBAUD = 0x0000;
	SciaRegs.SCILBAUD = SCI_PRD;
	SciaRegs.SCICCR.bit.LOOPBKENA = 1; // Enable loop back
	SciaRegs.SCIFFTX.all = 0xC020; //0 to set TX buffer 4 word to trigger the interupt &sciaTxFifoIsr;
	SciaRegs.SCIFFRX.all = 0x0024; //4 to set RX buffer 4 word to trigger the interupt &sciaRxFifoIsr;
	SciaRegs.SCIFFCT.all = 0x00;

	SciaRegs.SCICTL1.all = 0x0023;     // Relinquish SCI from Reset
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;

// Init send data.  After each transmission this data
// will be updated for the next transmission

//Inital the sending data pointer

	error_count = 0;
	success_count = 0;
	curr_Tx_pos = 0;
// Enable interrupts required for this example
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;     // PIE Group 9, INT1
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;     // PIE Group 9, INT2
	IER = 0x100;	// Enable CPU INT
	EINT;
//initial crc table
	crcInit();
//inital the table:

	uint8_t data[16] = {0x01, 0x02, 0x03, 0x04,
						0x01, 0x02, 0x03, 0x04,
						0x01, 0x02, 0x03, 0x04,
						0x01, 0x02, 0x03, 0x04 // P0 12 34 5+CF
			};
	int i;
	int DATA_PKT_MAXLEN = sizeof(data);
	for (i = 0; i < HDLC_PKT_MAXLEN; i++) {

		if(i < DATA_PKT_MAXLEN){
			table.pck[i] = data[i];
		} else
		{
			table.pck[i] = 0;
		}
	}
	table.len = DATA_PKT_MAXLEN;
	HDLC_encode(&table, &send);
	crc crcR = crcFast(table.pck, table.len);	//computer
}
