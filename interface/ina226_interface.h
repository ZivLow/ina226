/*
 *    Copyright 2024 Ziv Low
 *    
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *    
 *        http://www.apache.org/licenses/LICENSE-2.0
 *    
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once

#include "ina226_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/i2c_master.h"
#include <string>


#define DEFAULT_INA226_I2C_ADDRESS 0x40

/**
 * @brief Represents an INA226 device for current and power monitoring.
 * 
 * This class provides an interface for interacting with the INA226 device
 * over I2C communication. It allows initializing the I2C bus, writing to
 * and reading from the device registers, and handling I2C-related errors.
 */
class INA226 : public INA226_Driver {
public:
    /**
     * @brief Initializes the I2C of ESP-IDF
     * 
     * @param[in] sda_io_num GPIO number of SDA pin
     * @param[in] scl_io_num GPIO number of SCL pin
     * @param[in] address I2C address of INA226
     * @return std::expected<void, std::runtime_error> 
     */
    std::expected<void, std::runtime_error> I2C_Init(gpio_num_t sda_io_num = GPIO_NUM_21, gpio_num_t scl_io_num = GPIO_NUM_22, uint16_t address = DEFAULT_INA226_I2C_ADDRESS);

protected:
    /**
     * @brief I2C write function for ESP-IDF. Converts from little endian (ESP-IDF) to big endian (INA226).
     * 
     * @param[in] Register The register to write to on INA226
     * @param[in] Value The value to write to the register
     * @return std::expected<void, std::runtime_error> 
     */
    std::expected<void, std::runtime_error> I2C_Write(const Register Register, const uint16_t Value) override;

    /**
     * @brief I2C read function for ESP-IDF. Converts from big endian (INA226) to little endian (ESP-IDF).
     * 
     * @param[in] Register The register to read from on INA226
     * @return std::expected<uint16_t, std::runtime_error> 
     */
    std::expected<uint16_t, std::runtime_error> I2C_Read(const Register Register) override;

    /**
     * @brief Handles I2C related errors.
     * 
     * @param[in] operation The operation that caused the error
     * @param[in] err The error code
     * @return std::expected<void, std::runtime_error>
     */
    std::expected<void, std::runtime_error> HandleI2CError(const std::string &operation, esp_err_t err);

    /**
     * @brief Creates a mutex for I2C bus
     * 
     * @param[out] mutex The mutex to create
     * @return esp_err_t 
     */
    esp_err_t CreateMutex(SemaphoreHandle_t &mutex);

protected:
    SemaphoreHandle_t Lock;
    i2c_master_bus_config_t i2c_bus_config;
    i2c_master_bus_handle_t i2c_bus_handle;
    i2c_device_config_t i2c_dev_cfg;
    i2c_master_dev_handle_t i2c_dev_handle;
};