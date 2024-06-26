/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim8;

/* USER CODE BEGIN PV */
    int32_t sensor_read = 0x00000000;
	int position;
	int recv[100];
	float Kp = 1;// // 1 set up the constants value1.9//0.95//3
	//float Ki = 0.5;//0.5//0.3//0.468
	float Kd = 0.65;//22//25//20 0.04875//0.052

	float Kr = 0;
	int P, I, D, R;
	int lastError = 0;
	int errors[10] = {0,0,0,0,0,0,0,0,0,0};
	int error_sum = 0;
	int last_end = 0;	// 0 -> Left, 1 -> Right
	int last_idle = 0;
int c1,c2,c3,c4,c5,c6,c7,c8,c9,c10;
	const uint16_t Speed =1000;
	const uint16_t Speedt =1000;
    uint8_t c=0;
    uint8_t flag=0;
	int actives = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM8_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim2,0);  // set the counter value a 0
 while (__HAL_TIM_GET_COUNTER(&htim2) < us);  // wait for the counter to reach the us input in the parameter
}

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}





int QTR8_read_black()
{
	HAL_GPIO_WritePin(LEDON_GPIO_Port, LEDON_Pin, 1);

	//Set_Pin_Output(SENSOR1_GPIO_Port, SENSOR1_Pin);
	Set_Pin_Output(SENSOR2_GPIO_Port, SENSOR2_Pin);
	Set_Pin_Output(SENSOR3_GPIO_Port, SENSOR3_Pin);
	Set_Pin_Output(SENSOR4_GPIO_Port, SENSOR4_Pin);
	Set_Pin_Output(SENSOR5_GPIO_Port, SENSOR5_Pin);
	Set_Pin_Output(SENSOR6_GPIO_Port, SENSOR6_Pin);
	Set_Pin_Output(SENSOR7_GPIO_Port, SENSOR7_Pin);
	Set_Pin_Output(SENSOR8_GPIO_Port, SENSOR8_Pin);
	Set_Pin_Output(SENSOR9_GPIO_Port, SENSOR9_Pin);
	//Set_Pin_Output(SENSOR10_GPIO_Port, SENSOR10_Pin);

	//HAL_GPIO_WritePin (SENSOR1_GPIO_Port, SENSOR1_Pin, 1);
	HAL_GPIO_WritePin (SENSOR2_GPIO_Port, SENSOR2_Pin, 1);
	HAL_GPIO_WritePin (SENSOR3_GPIO_Port, SENSOR3_Pin, 1);
	HAL_GPIO_WritePin (SENSOR4_GPIO_Port, SENSOR4_Pin, 1);
	HAL_GPIO_WritePin (SENSOR5_GPIO_Port, SENSOR5_Pin, 1);
	HAL_GPIO_WritePin (SENSOR6_GPIO_Port, SENSOR6_Pin, 1);
	HAL_GPIO_WritePin (SENSOR7_GPIO_Port, SENSOR7_Pin, 1);
	HAL_GPIO_WritePin (SENSOR8_GPIO_Port, SENSOR8_Pin, 1);
	HAL_GPIO_WritePin (SENSOR9_GPIO_Port, SENSOR9_Pin, 1);
	//HAL_GPIO_WritePin (SENSOR10_GPIO_Port, SENSOR10_Pin, 1);
	delay_us(10);

	Set_Pin_Input(SENSOR1_GPIO_Port, SENSOR1_Pin);
	Set_Pin_Input(SENSOR2_GPIO_Port, SENSOR2_Pin);
	Set_Pin_Input(SENSOR3_GPIO_Port, SENSOR3_Pin);
	Set_Pin_Input(SENSOR4_GPIO_Port, SENSOR4_Pin);
	Set_Pin_Input(SENSOR5_GPIO_Port, SENSOR5_Pin);
	Set_Pin_Input(SENSOR6_GPIO_Port, SENSOR6_Pin);
	Set_Pin_Input(SENSOR7_GPIO_Port, SENSOR7_Pin);
	Set_Pin_Input(SENSOR8_GPIO_Port, SENSOR8_Pin);
	Set_Pin_Input(SENSOR9_GPIO_Port, SENSOR9_Pin);
	Set_Pin_Input(SENSOR10_GPIO_Port, SENSOR10_Pin);
	// Threshold
	delay_us(6000);

	sensor_read = 0x00000000;
	int pos = 0;
    int active = 0;

	if (HAL_GPIO_ReadPin(SENSOR2_GPIO_Port, SENSOR2_Pin)) {
		sensor_read |= 0x00000001;
		pos += 1000;

        active++;

	}
	if (HAL_GPIO_ReadPin(SENSOR3_GPIO_Port, SENSOR3_Pin)) {
		sensor_read |= 0x00000010;
		pos += 2000;
    active++;

  }
	if (HAL_GPIO_ReadPin(SENSOR4_GPIO_Port, SENSOR4_Pin)) {
		sensor_read |= 0x00000100;
		pos += 3000;
    active++;
  }
	if (HAL_GPIO_ReadPin(SENSOR5_GPIO_Port, SENSOR5_Pin)) {
		sensor_read |= 0x00001000;
		pos += 4000;
    active++;
  }
	if (HAL_GPIO_ReadPin(SENSOR6_GPIO_Port, SENSOR6_Pin)) {
		sensor_read |= 0x00010000;
		pos += 5000;
    active++;
  }
	if (HAL_GPIO_ReadPin(SENSOR7_GPIO_Port, SENSOR7_Pin)) {
		sensor_read |= 0x00100000;
		pos += 6000;
    active++;
  }
	if (HAL_GPIO_ReadPin(SENSOR8_GPIO_Port, SENSOR8_Pin)) {
		sensor_read |= 0x01000000;
		pos += 7000;
    active++;
  }

	if (HAL_GPIO_ReadPin(SENSOR9_GPIO_Port, SENSOR9_Pin)) {
		sensor_read |= 0x10000000;
		pos += 8000;
    active++;

  }


  HAL_GPIO_WritePin(LEDON_GPIO_Port, LEDON_Pin, 0);
    actives = active;
	position = pos/active;

	if (actives == 0)
		last_idle++;
	else
		last_idle = 0;
	
	c10=HAL_GPIO_ReadPin(SENSOR10_GPIO_Port, SENSOR10_Pin);
	c5=HAL_GPIO_ReadPin(SENSOR1_GPIO_Port, SENSOR1_Pin);
	return pos/active;
}


void past_errors (int error)
{
  for (int i = 9; i > 0; i--)
      errors[i] = errors[i-1];
  errors[0] = error;
}

int errors_sum (int index, int abs)
{
  int sum = 0;
  for (int i = 0; i < index; i++)
  {
    if ((abs == 1) && ( errors[i]< 0))
      sum += -errors[i];
    else
      sum += errors[i];
  }
  return sum;
}
void move(int motor, int speed, int direction)
{

    unsigned int inPin1 = 0;
    unsigned int inPin2 = 0;

    if (direction == 1)
    {
        inPin1 = speed;
        inPin2 = 0;
    }
    if (direction == 0)
    {
        inPin1 = 0;
        inPin2 = speed;
    }

    if (motor == 0)
    {
    	  __HAL_TIM_SET_COMPARE (&htim1, TIM_CHANNEL_1, inPin1);
    	  __HAL_TIM_SET_COMPARE (&htim1, TIM_CHANNEL_2,inPin2);

    }
    if (motor == 1)
    {
  	  __HAL_TIM_SET_COMPARE (&htim8, TIM_CHANNEL_2, inPin1*0.55);
  	  __HAL_TIM_SET_COMPARE (&htim8, TIM_CHANNEL_3,inPin2*0.55 );
    }
}

int constrain(int value, int min, int max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

void stop (void)
{

if((sensor_read==0x11111111)) {flag=1;}

if ((sensor_read!=0x11111111)&&(flag==1))
{
	__HAL_TIM_SET_COMPARE (&htim1, TIM_CHANNEL_1, 100);
	__HAL_TIM_SET_COMPARE (&htim1, TIM_CHANNEL_2,100);
    __HAL_TIM_SET_COMPARE (&htim8, TIM_CHANNEL_2, 100);
	__HAL_TIM_SET_COMPARE (&htim8, TIM_CHANNEL_3, 100);
    HAL_Delay(70000);
}
}





void PID_control(){
	uint16_t position=QTR8_read_black();
		int error=4333 - position;//5500//5300//4333
			past_errors(error);
			P = error;
			I = errors_sum(5, 0);
			D = error - lastError;
			R = errors_sum(5, 1);

		 int adjustment = Kp * error + Kd * D;
		 lastError = error;
c4=HAL_GPIO_ReadPin(SENSOR11_GPIO_Port, SENSOR11_Pin);
		 stop();

	        move(0, (constrain(Speed + adjustment, 0, Speed)), 1);
	        move(1, (constrain(Speed - adjustment, 0, Speed)), 1);
	/*int motorspeed = P*Kp + I*Ki + D*Kd;
*/



}

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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,1);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,1);
  while(!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))){}
  HAL_Delay(100);
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if ((HAL_GPIO_ReadPin(SENSOR1_GPIO_Port, SENSOR1_Pin))&&(!(HAL_GPIO_ReadPin(SENSOR10_GPIO_Port, SENSOR10_Pin))))
	  				 {
	  		 			do {c1=1;
	  		 			c3=0;
	  		 				move(0,Speedt, 1);
	  		 			}while(HAL_GPIO_ReadPin(SENSOR11_GPIO_Port, SENSOR11_Pin));

	  		 			c1=0; }
	  		 else if ((HAL_GPIO_ReadPin(SENSOR10_GPIO_Port, SENSOR10_Pin))&&(!(HAL_GPIO_ReadPin(SENSOR1_GPIO_Port, SENSOR1_Pin))))
	  		 			{
	  do{c2=1;
	  c3=0;
	  		 		move(1,Speedt, 1);
	  		 		}while(HAL_GPIO_ReadPin(SENSOR11_GPIO_Port, SENSOR11_Pin));
	  c2=0;}
	  		 else{c3=1;

	 PID_control();
	  		 }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV16;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 23;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 20;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 23;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 999;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */
  HAL_TIM_MspPostInit(&htim8);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SENSOR2_Pin|LEDON_Pin|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SENSOR3_Pin|SENSOR4_Pin|SENSOR5_Pin|SENSOR6_Pin
                          |SENSOR7_Pin|SENSOR8_Pin|SENSOR9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pins : SENSOR11_Pin SENSOR1_Pin */
  GPIO_InitStruct.Pin = SENSOR11_Pin|SENSOR1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SENSOR2_Pin LEDON_Pin PC4 PC5 */
  GPIO_InitStruct.Pin = SENSOR2_Pin|LEDON_Pin|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA9 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_9|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SENSOR3_Pin SENSOR4_Pin SENSOR5_Pin SENSOR6_Pin
                           SENSOR7_Pin SENSOR8_Pin SENSOR9_Pin */
  GPIO_InitStruct.Pin = SENSOR3_Pin|SENSOR4_Pin|SENSOR5_Pin|SENSOR6_Pin
                          |SENSOR7_Pin|SENSOR8_Pin|SENSOR9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SENSOR10_Pin */
  GPIO_InitStruct.Pin = SENSOR10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SENSOR10_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
