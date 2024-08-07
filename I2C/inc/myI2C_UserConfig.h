/*
 * myI2C_UserConfig.h
 *
 *  Created on: Aug 7, 2024
 *      Author: Le Anh
 */

#ifndef INC_MYI2C_USERCONFIG_H_
#define INC_MYI2C_USERCONFIG_H_

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM 			100000
#define I2C_SCL_SPEED_FM4K 			400000
#define I2C_SCL_SPEED_FM2K  		200000


/*
 * @I2C_AckControl
 */
typedef enum{
	I2C_ACK_DISABLE = 0U,
	I2C_ACK_ENABLE  = 1U
}I2C_ACKControl_t;


/*
 * @I2C_FMDutyCycle
 */
typedef enum{
	I2C_FM_DUTY_2 				= 0U,
	I2C_FM_DUTY_16_9     		= 1U
}I2C_FMDutyCycle_t;


#endif /* INC_MYI2C_USERCONFIG_H_ */
