/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define DHT11_Pin GPIO_PIN_14
#define DHT11_GPIO_Port GPIOC
#define R1_Pin GPIO_PIN_0
#define R1_GPIO_Port GPIOA
#define R2_Pin GPIO_PIN_1
#define R2_GPIO_Port GPIOA
#define ASRPRO_TX_Pin GPIO_PIN_2
#define ASRPRO_TX_GPIO_Port GPIOA
#define ASRPRO_RX_Pin GPIO_PIN_3
#define ASRPRO_RX_GPIO_Port GPIOA
#define L3_Pin GPIO_PIN_10
#define L3_GPIO_Port GPIOB
#define L4_Pin GPIO_PIN_11
#define L4_GPIO_Port GPIOB
#define TRIG_Pin GPIO_PIN_15
#define TRIG_GPIO_Port GPIOB
#define ECHO_Pin GPIO_PIN_8
#define ECHO_GPIO_Port GPIOA
#define USART_EN_Pin GPIO_PIN_12
#define USART_EN_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
