/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adafruit_APDS9960.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
APDS9960_Handle_t apds;

volatile HAL_StatusTypeDef i2c_status;
volatile uint8_t chip_id = 0;

// Détection de proximité
volatile uint8_t prox_val = 0;

// Détection de couleurs
volatile uint16_t r_val = 0;
volatile uint16_t g_val = 0;
volatile uint16_t b_val = 0;
volatile uint16_t clear_val = 0;

volatile float r_percent = 0.0;
volatile float g_percent = 0.0;
volatile float b_percent = 0.0;

// Code couleur détecté : 0 = Aucune, 1 = Rouge, 2 = Bleu, 3 = Vert
volatile uint8_t detected_color = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
    uint8_t c = (uint8_t)ch;
    HAL_UART_Transmit(&huart2, &c, 1, HAL_MAX_DELAY);
    return ch;
}

/*static void uart_print(const char *s) {
	HAL_UART_Transmit(&huart2, (uint8_t*) s, strlen(s), 100);
}*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_TIM15_Init();
  MX_USB_PCD_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // Vérification si le composant répond sur le bus I2C2
  i2c_status = HAL_I2C_IsDeviceReady(&hi2c2, APDS9960_ADDRESS, 3, 100);
  if (i2c_status != HAL_OK) {
	  printf("ERREUR : Problème avec la connexion i2c\r\n");
  } else {
	  printf("Connexion i2c établie !\r\n");
  }

  // Initialisation du capteur apds
  if (!APDS9960_Begin(&apds, &hi2c2, 10, APDS9960_AGAIN_4X)) {
	  printf("ERREUR : Problème d'initialisation de l'APDS-9960 (ID inconnu ou échec I2C).\r\n");
  } else {
	  printf("SUCCÈS : APDS-9960 initialisé avec succès (ID = 0xAB).\r\n");
  }

  // Lire le registre d'identification (ID = 0x92, valeur attendue = 0xAB)
  HAL_I2C_Mem_Read(&hi2c2, APDS9960_ADDRESS, 0x92, I2C_MEMADD_SIZE_8BIT,
		  (uint8_t*) &chip_id, 1, 100);

  // Configuration de la proximité (8 impulsions de 16 µs, gain xX)
  APDS9960_SetProxPulse(&apds, APDS9960_PPULSELEN_16US, 8);
  APDS9960_SetProxGain(&apds, APDS9960_PGAIN_8X);

  // Activation des modules Proximité et Couleur
  APDS9960_EnableProximity(&apds, true);
  APDS9960_EnableColor(&apds, true);

  HAL_Delay(10);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {

	  // Lecture valeur de proximite
	  prox_val = APDS9960_ReadProximity(&apds);
	  printf("Valeur proximite : %d \r\n", (int)prox_val);

	  // Traitement des couleurs si une canette est proche
	  if (prox_val > 50) {
		  if (APDS9960_ColorDataReady(&apds)) {

			  // Lecture des 4 canaux RGB + Clear (Luminosite)
			  APDS9960_GetColorData(&apds, (uint16_t*)&r_val,
					  (uint16_t*)&g_val,
					  (uint16_t*)&b_val,
					  (uint16_t*)&clear_val);

			  // Calcul des pourcentages relatifs
			  uint32_t total = r_val + g_val + b_val;
			  if (total > 0) {
				  r_percent = ((float)r_val / total) * 100.0f;
				  g_percent = ((float)g_val / total) * 100.0f;
				  b_percent = ((float)b_val / total) * 100.0f;
			  }

			  // Décision basée sur la couleur dominante
			  if (r_percent > g_percent && r_percent > b_percent && r_percent > 38.0f) {
				  detected_color = 1; // Rouge (Rouge prédominant)
			  }
			  else if (b_percent > r_percent && b_percent > g_percent && b_percent > 35.0f) {
				  detected_color = 2; // Bleu (Bleu prédominant)
			  }
			  else if (g_percent > r_percent && g_percent > b_percent) {
				  detected_color = 3; // Vert (Vert supérieur au Rouge ET au Bleu)
			  }
			  else {
				  detected_color = 0; // Inconnu
			  }
		  }
	  } else {
		  detected_color = 0; // Aucun objet proche
	  }

	  printf("R: %2.0f%% | G: %2.0f%% | B: %2.0f%% | Struct: %d\r\n",
	         r_percent, g_percent, b_percent, detected_color);
	  HAL_Delay(500);
	  /* USER CODE END WHILE */

	  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_CRSInitTypeDef pInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the SYSCFG APB clock
  */
  __HAL_RCC_CRS_CLK_ENABLE();

  /** Configures CRS
  */
  pInit.Prescaler = RCC_CRS_SYNC_DIV1;
  pInit.Source = RCC_CRS_SYNC_SOURCE_USB;
  pInit.Polarity = RCC_CRS_SYNC_POLARITY_RISING;
  pInit.ReloadValue = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000,1000);
  pInit.ErrorLimitValue = 34;
  pInit.HSI48CalibrationValue = 32;

  HAL_RCCEx_CRSConfig(&pInit);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
