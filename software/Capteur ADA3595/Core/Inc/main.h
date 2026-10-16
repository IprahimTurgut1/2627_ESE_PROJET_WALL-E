/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

#include "stm32g4xx_nucleo.h"

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
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOF
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOF
#define ENCA_BOUT_Pin GPIO_PIN_0
#define ENCA_BOUT_GPIO_Port GPIOA
#define ENCB_BOUT_Pin GPIO_PIN_1
#define ENCB_BOUT_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_3
#define VCP_RX_GPIO_Port GPIOA
#define LED_STATUS_Pin GPIO_PIN_5
#define LED_STATUS_GPIO_Port GPIOA
#define ENCA_AOUT_Pin GPIO_PIN_6
#define ENCA_AOUT_GPIO_Port GPIOA
#define ENCB_AOUT_Pin GPIO_PIN_7
#define ENCB_AOUT_GPIO_Port GPIOA
#define TOF_R_XSHUT_Pin GPIO_PIN_5
#define TOF_R_XSHUT_GPIO_Port GPIOC
#define APDS_INT_Pin GPIO_PIN_0
#define APDS_INT_GPIO_Port GPIOB
#define TOF_L_GPIO1_Pin GPIO_PIN_1
#define TOF_L_GPIO1_GPIO_Port GPIOB
#define TOF_L_XSHUT_Pin GPIO_PIN_2
#define TOF_L_XSHUT_GPIO_Port GPIOB
#define CHG_INT_Pin GPIO_PIN_10
#define CHG_INT_GPIO_Port GPIOB
#define CHG_INT_EXTI_IRQn EXTI15_10_IRQn
#define CHG_PG_Pin GPIO_PIN_12
#define CHG_PG_GPIO_Port GPIOB
#define TOF_R_GPIO1_Pin GPIO_PIN_13
#define TOF_R_GPIO1_GPIO_Port GPIOB
#define NEOPIXEL_Pin GPIO_PIN_14
#define NEOPIXEL_GPIO_Port GPIOB
#define CHG_PGB15_Pin GPIO_PIN_15
#define CHG_PGB15_GPIO_Port GPIOB
#define MOT_AIN1_Pin GPIO_PIN_6
#define MOT_AIN1_GPIO_Port GPIOC
#define MOT_AIN2_Pin GPIO_PIN_7
#define MOT_AIN2_GPIO_Port GPIOC
#define MOT_BIN1_Pin GPIO_PIN_8
#define MOT_BIN1_GPIO_Port GPIOC
#define MOT_BIN2_Pin GPIO_PIN_9
#define MOT_BIN2_GPIO_Port GPIOC
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define MOT_nSLEEP_Pin GPIO_PIN_10
#define MOT_nSLEEP_GPIO_Port GPIOC
#define MOT_nFAULT_Pin GPIO_PIN_11
#define MOT_nFAULT_GPIO_Port GPIOC
#define T_SWO_Pin GPIO_PIN_3
#define T_SWO_GPIO_Port GPIOB
#define GAUGE_INT_Pin GPIO_PIN_5
#define GAUGE_INT_GPIO_Port GPIOB
#define SERVO_PWM_Pin GPIO_PIN_6
#define SERVO_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
