/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usb_otg.c
  * @brief   This file provides code for the configuration
  *          of the USB_OTG instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usb_otg.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

PCD_HandleTypeDef hpcd_USB_OTG_HS;

/* USB_OTG_HS init function */

void MX_USB_OTG_HS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_HS_Init 0 */

  /* USER CODE END USB_OTG_HS_Init 0 */

  /* USER CODE BEGIN USB_OTG_HS_Init 1 */

  /* USER CODE END USB_OTG_HS_Init 1 */
  hpcd_USB_OTG_HS.Instance = USB_OTG_HS;
  hpcd_USB_OTG_HS.Init.dev_endpoints = 8;
  hpcd_USB_OTG_HS.Init.speed = PCD_SPEED_HIGH;
  hpcd_USB_OTG_HS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.phy_itface = USB_OTG_ULPI_PHY;
  hpcd_USB_OTG_HS.Init.Sof_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.vbus_sensing_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.use_dedicated_ep1 = DISABLE;
  hpcd_USB_OTG_HS.Init.use_external_vbus = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_HS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_HS_Init 2 */

  /* USER CODE END USB_OTG_HS_Init 2 */

}

void HAL_PCD_MspInit(PCD_HandleTypeDef* pcdHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(pcdHandle->Instance==USB_OTG_HS)
  {
  /* USER CODE BEGIN USB_OTG_HS_MspInit 0 */

  /* USER CODE END USB_OTG_HS_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USB_OTG_HS GPIO Configuration
    PB5     ------> USB_OTG_HS_ULPI_D7
    PH4     ------> USB_OTG_HS_ULPI_NXT
    PB13     ------> USB_OTG_HS_ULPI_D6
    PB12     ------> USB_OTG_HS_ULPI_D5
    PC0     ------> USB_OTG_HS_ULPI_STP
    PC2     ------> USB_OTG_HS_ULPI_DIR
    PA5     ------> USB_OTG_HS_ULPI_CK
    PB10     ------> USB_OTG_HS_ULPI_D3
    PA3     ------> USB_OTG_HS_ULPI_D0
    PB1     ------> USB_OTG_HS_ULPI_D2
    PB0     ------> USB_OTG_HS_ULPI_D1
    PB11     ------> USB_OTG_HS_ULPI_D4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_10
                          |GPIO_PIN_1|GPIO_PIN_0|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USB_OTG_HS clock enable */
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();
  /* USER CODE BEGIN USB_OTG_HS_MspInit 1 */

  /* USER CODE END USB_OTG_HS_MspInit 1 */
  }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* pcdHandle)
{

  if(pcdHandle->Instance==USB_OTG_HS)
  {
  /* USER CODE BEGIN USB_OTG_HS_MspDeInit 0 */

  /* USER CODE END USB_OTG_HS_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
    __HAL_RCC_USB_OTG_HS_ULPI_CLK_DISABLE();

    /**USB_OTG_HS GPIO Configuration
    PB5     ------> USB_OTG_HS_ULPI_D7
    PH4     ------> USB_OTG_HS_ULPI_NXT
    PB13     ------> USB_OTG_HS_ULPI_D6
    PB12     ------> USB_OTG_HS_ULPI_D5
    PC0     ------> USB_OTG_HS_ULPI_STP
    PC2     ------> USB_OTG_HS_ULPI_DIR
    PA5     ------> USB_OTG_HS_ULPI_CK
    PB10     ------> USB_OTG_HS_ULPI_D3
    PA3     ------> USB_OTG_HS_ULPI_D0
    PB1     ------> USB_OTG_HS_ULPI_D2
    PB0     ------> USB_OTG_HS_ULPI_D1
    PB11     ------> USB_OTG_HS_ULPI_D4
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_10
                          |GPIO_PIN_1|GPIO_PIN_0|GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_4);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_2);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_3);

  /* USER CODE BEGIN USB_OTG_HS_MspDeInit 1 */

  /* USER CODE END USB_OTG_HS_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */