/**
  ******************************************************************************
  * @file    usbd_desc_template.c
  * @author  MCD Application Team
  * @brief   This file provides the USBD descriptors and string formatting method.
  *          This template should be copied to the user folder,
  *          renamed and customized following user needs.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"
#include "usbd_custom_class.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USBD_VID                      0x0483
#define USBD_PID                      0x0108  								/* Random Product ID not used by ST */
#define USBD_LANGID_STRING            0x0409  								/* English (US) */
#define USBD_MANUFACTURER_STRING      "FB" 									/* Add your manufacturer string */
#define USBD_PRODUCT_HS_STRING        "CMSIS-DAP w/ bulk EPs" 				/* Add your product High Speed string */
#define USBD_PRODUCT_FS_STRING        "CMSIS-DAP w/ bulk EPs FS" 			/* Add your product Full Speed string */
#define USBD_CONFIGURATION_HS_STRING  "Config 1 HS" 						/* Add your configuration High Speed string */
#define USBD_INTERFACE_HS_STRING      "Tripple bulk EPs, no composite" 		/* Add your Interface High Speed string */
#define USBD_CONFIGURATION_FS_STRING  "Config 1 FS" 						/* Add your configuration Full Speed string */
#define USBD_INTERFACE_FS_STRING      "Tripple bulk EPs, no composite FS" 	/* Add your Interface Full Speed string */

#define DESC_TYPE_DEVICE_CAPABILITY	  16U
#define DEVICE_CAPABILITY_PLATFORM	  5U

/* nomenclature from usbd_def.h */
#define USB_LEN_MS_OS_DSC		      0xA2 	/* from picoprobe */
/* Private macro -------------------------------------------------------------*/

/* inspiration from TinyUSB*/
#define TU_U16(_high, _low)   ((uint16_t) (((_high) << 8) | (_low)))
#define TU_U16_HIGH(_u16)     ((uint8_t) (((_u16) >> 8) & 0x00ff))
#define TU_U16_LOW(_u16)      ((uint8_t) ((_u16)       & 0x00ff))
#define U16_TO_U8S_BE(_u16)   TU_U16_HIGH(_u16), TU_U16_LOW(_u16)
#define U16_TO_U8S_LE(_u16)   TU_U16_LOW(_u16), TU_U16_HIGH(_u16)

#define TU_U32_BYTE3(_u32)    ((uint8_t) ((((uint32_t) _u32) >> 24) & 0x000000ff)) // MSB
#define TU_U32_BYTE2(_u32)    ((uint8_t) ((((uint32_t) _u32) >> 16) & 0x000000ff))
#define TU_U32_BYTE1(_u32)    ((uint8_t) ((((uint32_t) _u32) >>  8) & 0x000000ff))
#define TU_U32_BYTE0(_u32)    ((uint8_t) (((uint32_t)  _u32)        & 0x000000ff)) // LSB

#define U32_TO_U8S_BE(_u32)   TU_U32_BYTE3(_u32), TU_U32_BYTE2(_u32), TU_U32_BYTE1(_u32), TU_U32_BYTE0(_u32)
#define U32_TO_U8S_LE(_u32)   TU_U32_BYTE0(_u32), TU_U32_BYTE1(_u32), TU_U32_BYTE2(_u32), TU_U32_BYTE3(_u32)

#define MS_OS_20_SET_HEADER_DESCRIPTOR       0x00
#define MS_OS_20_SUBSET_HEADER_CONFIGURATION 0x01
#define MS_OS_20_SUBSET_HEADER_FUNCTION      0x02
#define MS_OS_20_FEATURE_COMPATBLE_ID        0x03
#define MS_OS_20_FEATURE_REG_PROPERTY        0x04
#define MS_OS_20_FEATURE_MIN_RESUME_TIME     0x05
#define MS_OS_20_FEATURE_MODEL_ID            0x06
#define MS_OS_20_FEATURE_CCGP_DEVICE         0x07
#define MS_OS_20_FEATURE_VENDOR_REVISION     0x08

/* Private function prototypes -----------------------------------------------*/
uint8_t *USBD_Class_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_Class_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_Class_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_Class_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_Class_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_Class_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_Class_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_Class_MODStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);

#if (USBD_CLASS_USER_STRING_DESC == 1)
uint8_t *USBD_Class_UserStrDescriptor(USBD_SpeedTypeDef speed, uint8_t idx, uint16_t *length);
#endif /* USB_CLASS_USER_STRING_DESC */

#if ((USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1))
uint8_t *USBD_USR_BOSDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
#endif /* (USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1) */

/* Private variables ---------------------------------------------------------*/
USBD_DescriptorsTypeDef Class_Desc =
{
  USBD_Class_DeviceDescriptor,
  USBD_Class_LangIDStrDescriptor,
  USBD_Class_ManufacturerStrDescriptor,
  USBD_Class_ProductStrDescriptor,
  USBD_Class_SerialStrDescriptor,
  USBD_Class_ConfigStrDescriptor,
  USBD_Class_InterfaceStrDescriptor,
  USBD_Class_MODStrDescriptor,
#if (USBD_CLASS_USER_STRING_DESC == 1)
  USBD_Class_UserStrDescriptor,
#endif /* USB_CLASS_USER_STRING_DESC */

#if ((USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1))
  USBD_USR_BOSDescriptor,
#endif /* (USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1) */
};

/* USB Standard Device Descriptor */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif /* __ICCARM__ */
__ALIGN_BEGIN uint8_t USBD_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END =
{
  0x12,                       /* bLength */
  USB_DESC_TYPE_DEVICE,       /* bDescriptorType */
#if ((USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1))
  0x03, /*0x01,*/                       /*bcdUSB */     /* changed to USB version 2.01
                                              in order to support BOS Desc */
#else
  0x01, //0x00,                       /* bcdUSB */
#endif /* (USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1) */
  0x02,
  0xFF,						  /* bDeviceClass: vendor specific */
  0x00,                       /* bDeviceSubClass */
  0x00,                       /* bDeviceProtocol */
  USB_MAX_EP0_SIZE,           /* bMaxPacketSize */
  LOBYTE(USBD_VID),           /* idVendor */
  HIBYTE(USBD_VID),           /* idVendor */
  LOBYTE(0x5722+1), //LOBYTE(USBD_PID),           /* idProducz */
  HIBYTE(0x5722), //HIBYTE(USBD_PID),           /* idProduct */
  0x03,                       /* bcdDevice rel. 2.00, modyfied to 1.03 */
  0x01,
  USBD_IDX_MFC_STR,           /* Index of manufacturer string */
  USBD_IDX_PRODUCT_STR,       /* Index of product string */
  USBD_IDX_SERIAL_STR,        /* Index of serial number string */
  USBD_MAX_NUM_CONFIGURATION  /* bNumConfigurations */
}; /* USB_DeviceDescriptor */


/* USB Device LPM BOS descriptor */
#if (USBD_LPM_ENABLED == 1)
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif /* __ICCARM__ */
__ALIGN_BEGIN  uint8_t USBD_BOSDesc[USB_SIZ_BOS_DESC] __ALIGN_END =
{
  0x5,
  USB_DESC_TYPE_BOS,
  0xC,
  0x0,
  0x1,  /* 1 device capability */
  /* device capability */
  0x7,
  USB_DEVICE_CAPABITY_TYPE,
  0x2,
  0x6, /*LPM capability bit set */
  0x0,
  0x0,
  0x0
};
#endif /* USBD_LPM_ENABLED */

/* USB Device Billboard BOS descriptor Template */
#if (USBD_CLASS_BOS_ENABLED == 1)
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif /* __ICCARM__ */
__ALIGN_BEGIN  uint8_t USBD_BOSDesc[USB_SIZ_BOS_DESC] __ALIGN_END =
{
  0x05,                                /* bLength */
  USB_DESC_TYPE_BOS,                   /* Device Descriptor Type */
  LOBYTE(USB_SIZ_BOS_DESC),            /* Total length of BOS descriptor and all of its sub descs */
  HIBYTE(USB_SIZ_BOS_DESC),
  0x01,                                /* The number of separate device capability descriptors in the BOS */

  /* Device Capability Platform descriptor */
  0x1C,		   						   /* bLength = 28 bytes*/
  DESC_TYPE_DEVICE_CAPABILITY,		   /* bDescriptorType */
  DEVICE_CAPABILITY_PLATFORM,		   /* bDevCapabilityType */
  0x00,								   /* bReserved */
  0xDF, 0x60, 0xDD, 0xD8,		       /* MS_OS_20_Platform_Capability_ID = D8DD60DF-4589-4CC7-9CD2-659D9E648A9F */
  0x89, 0x45,						   /* 	the byte order flips midway in the comment but if you google  */
  0xC7, 0x4C,						   /* 	MS_OS_20_Platform_Capability_ID, all of the results are like this */
  0x9C, 0xD2, 						   /*	so I assume that the OG comment in the OG documentation if wrong */
  0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F,

  /*Descriptor Set information 1 */
  U32_TO_U8S_LE(0x06030000),		   /* dwWindowsVersion = Windows 8.1 or later */
  U16_TO_U8S_LE(USB_LEN_MS_OS_DSC),	   /* wMSOSDescriptorSetTotalLength */
  USB_REQ_MS_VendorCode,			   /* bMS_VendorCode */
  0x00,								   /* bAltEnumCode */

};
#endif /* USBD_CLASS_BOS_ENABLED */


/* USB Standard Device Descriptor */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif /* __ICCARM__ */
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
{
  USB_LEN_LANGID_STR_DESC,
  USB_DESC_TYPE_STRING,
  LOBYTE(USBD_LANGID_STRING),
  HIBYTE(USBD_LANGID_STRING),
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif /* __ICCARM__ */
__ALIGN_BEGIN uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] =
{
  USB_SIZ_STRING_SERIAL,
  USB_DESC_TYPE_STRING,
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif /* __ICCARM__ */
__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

/* following section is taken from picoprobe project: */


/* Microsoft OS 2.0 registry property descriptor
Per MS requirements https://msdn.microsoft.com/en-us/library/windows/hardware/hh450799(v=vs.85).aspx
device should create DeviceInterfaceGUIDs. It can be done by driver and
in case of real PnP solution device should expose MS "Microsoft OS 2.0
registry property descriptor". Such descriptor can insert any record
into Windows registry per device/configuration/interface. In our case it
will insert "DeviceInterfaceGUIDs" multistring property. */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif /* __ICCARM__ */
/* USB MS OS 2.0 (string) Descriptor */
__ALIGN_BEGIN static uint8_t USBD_TEMPLATE_MOD_STR[USB_LEN_MS_OS_DSC] __ALIGN_END =
{
  // Set header: length, type, windows version, total length
  U16_TO_U8S_LE(0x000A), U16_TO_U8S_LE(MS_OS_20_SET_HEADER_DESCRIPTOR), U32_TO_U8S_LE(0x06030000), U16_TO_U8S_LE(USB_LEN_MS_OS_DSC),

  // Configuration subset header: length, type, configuration index, reserved, configuration total length
  //U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_CONFIGURATION), 0, 0, U16_TO_U8S_LE(USB_LEN_MS_OS_DSC-0x0A),

  // Function Subset header: length, type, first interface, reserved, subset length
  //U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION), DAP_V2_IF_NUM , 0, U16_TO_U8S_LE(USB_LEN_MS_OS_DSC-0x0A-0x08), //TODO is this the correct IF number

  // MS OS 2.0 Compatible ID descriptor: length, type, compatible ID, sub compatible ID
  U16_TO_U8S_LE(0x0014), U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID), 'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // sub-compatible

  // MS OS 2.0 Registry property descriptor: length, type
  U16_TO_U8S_LE(USB_LEN_MS_OS_DSC-0x0A/*-0x08-0x08*/-0x14), U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY),
  U16_TO_U8S_LE(0x0007), U16_TO_U8S_LE(0x002A), // wPropertyDataType, wPropertyNameLength and PropertyName "DeviceInterfaceGUIDs\0" in UTF-16
  'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00,
  'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 0x00, 0x00,
  U16_TO_U8S_LE(0x0050), // wPropertyDataLength
  // bPropertyData "{CDB3B5AD-293B-4663-AA36-1AAE46463776}" as a UTF-16 string (b doesn't mean bytes)
  '{', 0x00, 'C', 0x00, 'D', 0x00, 'B', 0x00, '3', 0x00, 'B', 0x00, '5', 0x00, 'A', 0x00, 'D', 0x00, '-', 0x00,
  '2', 0x00, '9', 0x00, '3', 0x00, 'B', 0x00, '-', 0x00, '4', 0x00, '6', 0x00, '6', 0x00, '3', 0x00, '-', 0x00,
  'A', 0x00, 'A', 0x00, '3', 0x00, '6', 0x00, '-', 0x00, '1', 0x00, 'A', 0x00, 'A', 0x00, 'E', 0x00, '4', 0x00,
  '6', 0x00, '4', 0x00, '6', 0x00, '3', 0x00, '7', 0x00, '7', 0x00, '6', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00

};


/* Private functions ---------------------------------------------------------*/
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len);
static void Get_SerialNum(void);

/**
  * @brief  Returns the device descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Class_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);

  *length = sizeof(USBD_DeviceDesc);
  return (uint8_t *)USBD_DeviceDesc;
}

/**
  * @brief  Returns the LangID string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Class_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);

  *length = sizeof(USBD_LangIDDesc);
  return (uint8_t *)USBD_LangIDDesc;
}

/**
  * @brief  Returns the product string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Class_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if (speed == USBD_SPEED_HIGH)
  {
    USBD_GetString((uint8_t *)USBD_PRODUCT_HS_STRING, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_PRODUCT_FS_STRING, USBD_StrDesc, length);
  }
  return USBD_StrDesc;
}

/**
  * @brief  Returns the manufacturer string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Class_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);

  USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

/**
  * @brief  Returns the serial number string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Class_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);

  *length = USB_SIZ_STRING_SERIAL;

  /* Update the serial number string descriptor with the data from the unique ID*/
  Get_SerialNum();

  return (uint8_t *)USBD_StringSerial;
}

/**
  * @brief  Returns the configuration string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Class_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if (speed == USBD_SPEED_HIGH)
  {
    USBD_GetString((uint8_t *)USBD_CONFIGURATION_HS_STRING, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_CONFIGURATION_FS_STRING, USBD_StrDesc, length);
  }
  return USBD_StrDesc;
}

/**
  * @brief  Returns the interface string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Class_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if (speed == USBD_SPEED_HIGH)
  {
    USBD_GetString((uint8_t *)USBD_INTERFACE_HS_STRING, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_INTERFACE_FS_STRING, USBD_StrDesc, length);
  }
  return USBD_StrDesc;
}

/**
  * @brief  Returns the Microsoft OS (string) descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Class_MODStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length){
	  if (speed == USBD_SPEED_HIGH)
	  {
		  *length = sizeof(USBD_TEMPLATE_MOD_STR);
		   return (uint8_t *)USBD_TEMPLATE_MOD_STR;
	    //USBD_GetString((uint8_t *)USBD_TEMPLATE_MOD_STR, USBD_StrDesc, length);
	  }
	  else
	  {
		  *length = sizeof(USBD_TEMPLATE_MOD_STR);
		   return (uint8_t *)USBD_TEMPLATE_MOD_STR;
	    //USBD_GetString((uint8_t *)USBD_TEMPLATE_MOD_STR, USBD_StrDesc, length);
	  }
	  //return USBD_StrDesc;
}


/**
  * @brief  Create the serial number string descriptor
  * @param  None
  * @retval None
  */
static void Get_SerialNum(void)
{
  uint32_t deviceserial0, deviceserial1, deviceserial2;

  deviceserial0 = *(uint32_t *)DEVICE_ID1;
  deviceserial1 = *(uint32_t *)DEVICE_ID2;
  deviceserial2 = *(uint32_t *)DEVICE_ID3;

  deviceserial0 += deviceserial2;

  if (deviceserial0 != 0U)
  {
    IntToUnicode(deviceserial0, &USBD_StringSerial[2], 8U);
    IntToUnicode(deviceserial1, &USBD_StringSerial[18], 4U);
  }
}


#if ((USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1))
/**
  * @brief  USBD_USR_BOSDescriptor
  *         return the BOS descriptor
  * @param  speed : current device speed
  * @param  length : pointer to data length variable
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_USR_BOSDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  *length = sizeof(USBD_BOSDesc);
  return (uint8_t *)USBD_BOSDesc;
}
#endif /* (USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1) */


#if (USBD_CLASS_USER_STRING_DESC == 1)
/**
  * @brief  Returns the Class User string descriptor.
  * @param  speed: Current device speed
  * @param  idx: index of string descriptor
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Class_UserStrDescriptor(USBD_SpeedTypeDef speed, uint8_t idx, uint16_t *length)
{
  static uint8_t USBD_StrDesc[255];   //TODO this is probably useless in this form

  return USBD_StrDesc;
}
#endif /* USBD_CLASS_USER_STRING_DESC */


/**
  * @brief  Convert Hex 32Bits value into char
  * @param  value: value to convert
  * @param  pbuf: pointer to the buffer
  * @param  len: buffer length
  * @retval None
  */
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len)
{
  uint8_t idx = 0U;

  for (idx = 0U ; idx < len ; idx ++)
  {
    if (((value >> 28)) < 0xAU)
    {
      pbuf[ 2U * idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2U * idx] = (value >> 28) + 'A' - 10U;
    }

    value = value << 4;

    pbuf[2U * idx + 1] = 0U;
  }
}
