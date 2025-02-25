/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
volatile uint8_t ledState = 0;  // LED toggle state
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	 GPIO_Config();  // Initialize GPIOs
	 EXTI_Config();  // Configure External Interrupt (EXTI)

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */


  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */


  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */



/* USER CODE BEGIN 4 */
void GPIO_Config(void)
{
    // Enable clock for GPIOA
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Configure PA6 as output
    GPIOA->MODER |= (1 << (6 * 2));  // Set PA6 as output (01)
    GPIOA->MODER &= ~(1 << (6 * 2 + 1));

    // Configure PA7 as input
    GPIOA->MODER &= ~(3 << (7 * 2));  // Set PA7 as input (00)

        // Enable internal pull-up for PA7
    GPIOA->PUPDR |= (1 << (7 * 2));  // Pull-up (01)
    GPIOA->PUPDR &= ~(1 << (7 * 2 + 1));

}
// Configure EXTI for PA7 (EXTI7)
void EXTI_Config(void)
{
    // Enable SYSCFG clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
    // Map PA7 to EXTI7
     SYSCFG->EXTICR[1] &= ~(0xF << (3 * 4));  // Clear EXTI7 mapping
     SYSCFG->EXTICR[1] |= (0x0 << (3 * 4));   // Map EXTI7 to PA7
        // Enable falling-edge trigger for EXTI7
      EXTI->FTSR |= EXTI_FTSR_TR7;
      EXTI->RTSR &= ~EXTI_RTSR_TR7;  // Disable rising-edge trigger

      // Enable EXTI7 interrupt
          EXTI->IMR |= EXTI_IMR_IM7;
          // Enable EXTI4_15 interrupt in NVIC (PA7 is EXTI7, part of EXTI4_15 IRQ)
              NVIC_EnableIRQ(EXTI4_15_IRQn);
              NVIC_SetPriority(EXTI4_15_IRQn, 0);
          }
// EXTI7 Interrupt Handler (EXTI4_15 handles EXTI7)
void EXTI4_15_IRQHandler(void)
{

	    if (EXTI->PR & EXTI_PR_PR7)  // Check if EXTI7 triggered
	    {
	        ledState = !ledState;  // Toggle LED state
	        if (ledState){
	                GPIOA->BSRR = (1 << 6);  // Set PA6 (turn LED ON)
	        }
	        else{
	             GPIOA->BRR = (1 << 6);   // Reset PA6 (turn LED OFF)
	        }
	        EXTI->PR |= EXTI_PR_PR7;

}

}




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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
