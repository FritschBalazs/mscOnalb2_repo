/*
 * app.h
 *
 *  Created on: Apr 15, 2023
 *      Author: frici
 */

#ifndef INC_APP_H_
#define INC_APP_H_

enum USBD_HID_REQ_t {
	USBD_HID_REQ_EP_CTRL = 0,
	USBD_HID_REQ_EP_INT,
	USBD_HID_REQ_PERIOD_UPDATE
};

enum USB_HID_REPORT_TYP_t {
	HID_REPORT_INPUT,
	HID_REPORT_FEATURE,
	HID_REPORT_OUTPUT,
};


void DAP_USB_Initialize (void);

void APP_Run(void);
void USBD_InEvent(void);    //My Event handler
void USBD_OutEvent(void);	//My Event handler

#endif /* INC_APP_H_ */
