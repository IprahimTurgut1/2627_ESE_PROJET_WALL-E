/*
 * Adafruit_APDS9960.c
 *
 *  Created on: 7 oct. 2026
 *      Author: beaus
 */

#include "adafruit_APDS9960.h"
#include <math.h>

/* Fonctions internes d'accès I2C */
static void write8(APDS9960_Handle_t *dev, uint8_t reg, uint8_t value) {
  HAL_I2C_Mem_Write(dev->hi2c, APDS9960_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 100);
}

static uint8_t read8(APDS9960_Handle_t *dev, uint8_t reg) {
  uint8_t val = 0;
  HAL_I2C_Mem_Read(dev->hi2c, APDS9960_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
  return val;
}

static uint16_t read16R(APDS9960_Handle_t *dev, uint8_t reg) {
  uint8_t buf[2] = {0};
  HAL_I2C_Mem_Read(dev->hi2c, APDS9960_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, buf, 2, 100);
  return (uint16_t)((buf[1] << 8) | buf[0]);
}

/* Initialisation */
bool APDS9960_Begin(APDS9960_Handle_t *dev, I2C_HandleTypeDef *hi2c, uint16_t iTimeMS, apds9960AGain_t aGain) {
  dev->hi2c = hi2c;
  dev->enable_reg = 0;
  dev->control_reg = 0;
  dev->pers_reg = 0;
  dev->ppulse_reg = 0;
  dev->config2_reg = 0x01;

  /* Vérification de la présence du capteur */
  uint8_t id = read8(dev, APDS9960_ID);
  if (id != 0xAB) {
    return false;
  }

  /* Configuration du temps d'intégration de l'ADC */
  float temp = (float)iTimeMS / 2.78f;
  temp = 256.0f - temp;
  if (temp > 255.0f) temp = 255.0f;
  if (temp < 0.0f)   temp = 0.0f;
  write8(dev, APDS9960_ATIME, (uint8_t)temp);

  /* Configuration du Gain ADC */
  dev->control_reg = (dev->control_reg & ~0x03) | (aGain & 0x03);
  write8(dev, APDS9960_CONTROL, dev->control_reg);

  /* Désactivation globale initiale */
  write8(dev, APDS9960_ENABLE, 0x00);
  HAL_Delay(10);
  APDS9960_Enable(dev, true);
  HAL_Delay(10);

  /* Configuration des impulsions proximité par défaut (10 pulses à 32us) */
  dev->ppulse_reg = (APDS9960_PPULSELEN_32US << 6) | (9 & 0x3F);
  write8(dev, APDS9960_PPULSE, dev->ppulse_reg);

  return true;
}

void APDS9960_Enable(APDS9960_Handle_t *dev, bool en) {
  if (en) dev->enable_reg |= 0x01;  /* PON */
  else    dev->enable_reg &= ~0x01;
  write8(dev, APDS9960_ENABLE, dev->enable_reg);
}

void APDS9960_EnableProximity(APDS9960_Handle_t *dev, bool en) {
  if (en) dev->enable_reg |= 0x04;  /* PEN */
  else    dev->enable_reg &= ~0x04;
  write8(dev, APDS9960_ENABLE, dev->enable_reg);
}

void APDS9960_SetProxGain(APDS9960_Handle_t *dev, apds9960PGain_t gain) {
  dev->control_reg = (dev->control_reg & ~(0x03 << 2)) | ((gain & 0x03) << 2);
  write8(dev, APDS9960_CONTROL, dev->control_reg);
}

void APDS9960_SetProxPulse(APDS9960_Handle_t *dev, apds9960PPulseLen_t pLen, uint8_t pulses) {
  if (pulses < 1) pulses = 1;
  if (pulses > 64) pulses = 64;
  pulses--;
  dev->ppulse_reg = ((pLen & 0x03) << 6) | (pulses & 0x3F);
  write8(dev, APDS9960_PPULSE, dev->ppulse_reg);
}

void APDS9960_EnableProximityInterrupt(APDS9960_Handle_t *dev) {
  dev->enable_reg |= 0x20; /* PIEN */
  write8(dev, APDS9960_ENABLE, dev->enable_reg);
  APDS9960_ClearInterrupt(dev);
}

void APDS9960_DisableProximityInterrupt(APDS9960_Handle_t *dev) {
  dev->enable_reg &= ~0x20; /* PIEN */
  write8(dev, APDS9960_ENABLE, dev->enable_reg);
}

void APDS9960_SetProximityInterruptThreshold(APDS9960_Handle_t *dev, uint8_t low, uint8_t high, uint8_t persistence) {
  write8(dev, APDS9960_PILT, low);
  write8(dev, APDS9960_PIHT, high);

  if (persistence > 7) persistence = 7;
  dev->pers_reg = (dev->pers_reg & ~0xF0) | ((persistence & 0x0F) << 4);
  write8(dev, APDS9960_PERS, dev->pers_reg);
}

bool APDS9960_GetProximityInterrupt(APDS9960_Handle_t *dev) {
  uint8_t status = read8(dev, APDS9960_STATUS);
  return (status >> 5) & 0x01; /* PINT */
}

uint8_t APDS9960_ReadProximity(APDS9960_Handle_t *dev) {
  return read8(dev, APDS9960_PDATA);
}

void APDS9960_EnableColor(APDS9960_Handle_t *dev, bool en) {
  if (en) dev->enable_reg |= 0x02;  /* AEN */
  else    dev->enable_reg &= ~0x02;
  write8(dev, APDS9960_ENABLE, dev->enable_reg);
}

bool APDS9960_ColorDataReady(APDS9960_Handle_t *dev) {
  uint8_t status = read8(dev, APDS9960_STATUS);
  return (status & 0x01); /* AVALID */
}

void APDS9960_GetColorData(APDS9960_Handle_t *dev, uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
  *c = read16R(dev, APDS9960_CDATAL);
  *r = read16R(dev, APDS9960_RDATAL);
  *g = read16R(dev, APDS9960_GDATAL);
  *b = read16R(dev, APDS9960_BDATAL);
}

void APDS9960_ClearInterrupt(APDS9960_Handle_t *dev) {
  uint8_t dummy = 0;
  HAL_I2C_Mem_Read(dev->hi2c, APDS9960_ADDRESS, APDS9960_AICLEAR, I2C_MEMADD_SIZE_8BIT, &dummy, 1, 100);
}

void APDS9960_SetLED(APDS9960_Handle_t *dev, apds9960LedDrive_t drive, apds9960LedBoost_t boost) {
  dev->config2_reg = (dev->config2_reg & ~(0x03 << 4)) | ((boost & 0x03) << 4);
  write8(dev, APDS9960_CONFIG2, dev->config2_reg);

  dev->control_reg = (dev->control_reg & ~(0x03 << 6)) | ((drive & 0x03) << 6);
  write8(dev, APDS9960_CONTROL, dev->control_reg);
}

uint16_t APDS9960_CalculateColorTemperature(uint16_t r, uint16_t g, uint16_t b) {
  float X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
  float Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
  float Z = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

  if ((X + Y + Z) == 0.0f) return 0;

  float xc = X / (X + Y + Z);
  float yc = Y / (X + Y + Z);

  if ((0.1858F - yc) == 0.0f) return 0;

  float n = (xc - 0.3320F) / (0.1858F - yc);
  float cct = (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

  return (uint16_t)cct;
}

uint16_t APDS9960_CalculateLux(uint16_t r, uint16_t g, uint16_t b) {
  float illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
  if (illuminance < 0.0f) illuminance = 0.0f;
  return (uint16_t)illuminance;
}
