/*
 * adafruit_APDS9960.h
 *
 *  Created on: 7 oct. 2026
 *      Author: beaus
 */

#ifndef CAPTEUR_ADA3595_CORE_INC_ADAFRUIT_APDS9960_H_
#define CAPTEUR_ADA3595_CORE_INC_ADAFRUIT_APDS9960_H_

#include "stm32g4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define APDS9960_ADDRESS	(0x39 << 1) /**< Adresse I2C décalée pour STM32 HAL */

/** Registres I2C APDS9960 */
typedef enum {
  APDS9960_RAM        = 0x00,
  APDS9960_ENABLE     = 0x80,
  APDS9960_ATIME      = 0x81,
  APDS9960_WTIME      = 0x83,
  APDS9960_AILTIL     = 0x84,
  APDS9960_AILTH      = 0x85,
  APDS9960_AIHTL      = 0x86,
  APDS9960_AIHTH      = 0x87,
  APDS9960_PILT       = 0x89,
  APDS9960_PIHT       = 0x8B,
  APDS9960_PERS       = 0x8C,
  APDS9960_CONFIG1    = 0x8D,
  APDS9960_PPULSE     = 0x8E,
  APDS9960_CONTROL    = 0x8F,
  APDS9960_CONFIG2    = 0x90,
  APDS9960_ID         = 0x92,
  APDS9960_STATUS     = 0x93,
  APDS9960_CDATAL     = 0x94,
  APDS9960_CDATAH     = 0x95,
  APDS9960_RDATAL     = 0x96,
  APDS9960_RDATAH     = 0x97,
  APDS9960_GDATAL     = 0x98,
  APDS9960_GDATAH     = 0x99,
  APDS9960_BDATAL     = 0x9A,
  APDS9960_BDATAH     = 0x9B,
  APDS9960_PDATA      = 0x9C,
  APDS9960_POFFSET_UR = 0x9D,
  APDS9960_POFFSET_DL = 0x9E,
  APDS9960_CONFIG3    = 0x9F,
  APDS9960_GPENTH     = 0xA0,
  APDS9960_GEXTH      = 0xA1,
  APDS9960_GCONF1     = 0xA2,
  APDS9960_GCONF2     = 0xA3,
  APDS9960_GOFFSET_U  = 0xA4,
  APDS9960_GOFFSET_D  = 0xA5,
  APDS9960_GPULSE     = 0xA6,
  APDS9960_GOFFSET_L  = 0xA7,
  APDS9960_GOFFSET_R  = 0xA9,
  APDS9960_GCONF3     = 0xAA,
  APDS9960_GCONF4     = 0xAB,
  APDS9960_GFLVL      = 0xAE,
  APDS9960_GSTATUS    = 0xAF,
  APDS9960_IFORCE     = 0xE4,
  APDS9960_PICLEAR    = 0xE5,
  APDS9960_CICLEAR    = 0xE6,
  APDS9960_AICLEAR    = 0xE7,
  APDS9960_GFIFO_U    = 0xFC,
  APDS9960_GFIFO_D    = 0xFD,
  APDS9960_GFIFO_L    = 0xFE,
  APDS9960_GFIFO_R    = 0xFF
} apds9960_reg_t;

/** Gain ADC Couleur */
typedef enum {
  APDS9960_AGAIN_1X  = 0x00, // Pas de gain
  APDS9960_AGAIN_4X  = 0x01, // Gain x2
  APDS9960_AGAIN_16X = 0x02, // Gain 16x
  APDS9960_AGAIN_64X = 0x03 // Gain 64x
} apds9960AGain_t;

/** Gain Proximité */
typedef enum {
  APDS9960_PGAIN_1X  = 0x00, // Pas de gain
  APDS9960_PGAIN_2X  = 0x01, // 2x gain
  APDS9960_PGAIN_4X  = 0x02, // 4x gain
  APDS9960_PGAIN_8X  = 0x03 // 8x gain
} apds9960PGain_t;


/** Longueur d'impulsion Proximité */
typedef enum {
  APDS9960_PPULSELEN_4US = 0x00,  /**< 4uS */
  APDS9960_PPULSELEN_8US = 0x01,  /**< 8uS */
  APDS9960_PPULSELEN_16US = 0x02, /**< 16uS */
  APDS9960_PPULSELEN_32US = 0x03  /**< 32uS */
} apds9960PPulseLen_t;

/** Courant LED */
typedef enum {
  APDS9960_LEDDRIVE_100MA = 0x00, /**< 100mA */
  APDS9960_LEDDRIVE_50MA = 0x01,  /**< 50mA */
  APDS9960_LEDDRIVE_25MA = 0x02,  /**< 25mA */
  APDS9960_LEDDRIVE_12MA = 0x03   /**< 12.5mA */
} apds9960LedDrive_t;

/** Boost LED */
typedef enum {
  APDS9960_LEDBOOST_100PCNT = 0x00, /**< 100% */
  APDS9960_LEDBOOST_150PCNT = 0x01, /**< 150% */
  APDS9960_LEDBOOST_200PCNT = 0x02, /**< 200% */
  APDS9960_LEDBOOST_300PCNT = 0x03  /**< 300% */
} apds9960LedBoost_t;

/** Constantes de gestes */
#define APDS9960_UP 0x01    /**< Gesture Up */
#define APDS9960_DOWN 0x02  /**< Gesture Down */
#define APDS9960_LEFT 0x03  /**< Gesture Left */
#define APDS9960_RIGHT 0x04 /**< Gesture Right */

/** Dimensions */
enum {
  APDS9960_DIMENSIONS_ALL = 0x00,        // All dimensions
  APDS9960_DIMENSIONS_UP_DOWN = 0x01,    // Up/Down dimensions
  APDS9960_DIMENSIONS_LEFT_RIGHT = 0x02, // Left/Right dimensions
};

/** FIFO Interrupts */
enum {
  APDS9960_GFIFO_1 = 0x00,  // Generate interrupt after 1 dataset in FIFO
  APDS9960_GFIFO_4 = 0x01,  // Generate interrupt after 2 datasets in FIFO
  APDS9960_GFIFO_8 = 0x02,  // Generate interrupt after 3 datasets in FIFO
  APDS9960_GFIFO_16 = 0x03, // Generate interrupt after 4 datasets in FIFO
};

/** Gesture Gain */
enum {
  APDS9960_GGAIN_1 = 0x00, // Gain 1x
  APDS9960_GGAIN_2 = 0x01, // Gain 2x
  APDS9960_GGAIN_4 = 0x02, // Gain 4x
  APDS9960_GGAIN_8 = 0x03, // Gain 8x
};

/** Pulse Lenghts */
enum {
  APDS9960_GPULSE_4US = 0x00,  // Pulse 4us
  APDS9960_GPULSE_8US = 0x01,  // Pulse 8us
  APDS9960_GPULSE_16US = 0x02, // Pulse 16us
  APDS9960_GPULSE_32US = 0x03, // Pulse 32us
};

/** Handle d'instance APDS9960 **/
typedef struct {
  I2C_HandleTypeDef *hi2c;
  uint8_t enable_reg;
  uint8_t control_reg;
  uint8_t pers_reg;
  uint8_t ppulse_reg;
  uint8_t config2_reg;
} APDS9960_Handle_t;

/* Fonctions de base */
bool APDS9960_Begin(APDS9960_Handle_t *dev, I2C_HandleTypeDef *hi2c, uint16_t iTimeMS, apds9960AGain_t aGain);
void APDS9960_Enable(APDS9960_Handle_t *dev, bool en);

/* Fonctions Proximité */
void APDS9960_EnableProximity(APDS9960_Handle_t *dev, bool en);
void APDS9960_SetProxGain(APDS9960_Handle_t *dev, apds9960PGain_t gain);
void APDS9960_SetProxPulse(APDS9960_Handle_t *dev, apds9960PPulseLen_t pLen, uint8_t pulses);
void APDS9960_EnableProximityInterrupt(APDS9960_Handle_t *dev);
void APDS9960_DisableProximityInterrupt(APDS9960_Handle_t *dev);
uint8_t APDS9960_ReadProximity(APDS9960_Handle_t *dev);
void APDS9960_SetProximityInterruptThreshold(APDS9960_Handle_t *dev, uint8_t low, uint8_t high, uint8_t persistence);
bool APDS9960_GetProximityInterrupt(APDS9960_Handle_t *dev);

/* Fonctions Couleur / ALS */
void APDS9960_EnableColor(APDS9960_Handle_t *dev, bool en);
bool APDS9960_ColorDataReady(APDS9960_Handle_t *dev);
void APDS9960_GetColorData(APDS9960_Handle_t *dev, uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
uint16_t APDS9960_CalculateColorTemperature(uint16_t r, uint16_t g, uint16_t b);
uint16_t APDS9960_CalculateLux(uint16_t r, uint16_t g, uint16_t b);
void APDS9960_ClearInterrupt(APDS9960_Handle_t *dev);

/* Fonctions LED */
void APDS9960_SetLED(APDS9960_Handle_t *dev, apds9960LedDrive_t drive, apds9960LedBoost_t boost);



#endif /* CAPTEUR_ADA3595_CORE_INC_ADAFRUIT_APDS9960_H_ */
