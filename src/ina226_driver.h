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

#include <inttypes.h>
#include <expected>
#include <stdexcept>

class INA226_Driver
{
public:
    enum class OperatingMode : uint8_t {
        POWER_DOWN = 0,
        SHUNT_VOLTAGE_TRIGGERED,
        BUS_VOLTAGE_TRIGGERED,
        SHUNT_AND_BUS_TRIGGERED,
        ADC_OFF,
        SHUNT_VOLTAGE_CONTINUOUS,
        BUS_VOLTAGE_CONTINUOUS,
        SHUNT_AND_BUS_CONTINUOUS,
    };

    enum class AveragingMode : uint8_t {
        SAMPLE_1 = 0,
        SAMPLE_4,
        SAMPLE_16,
        SAMPLE_64,
        SAMPLE_128,
        SAMPLE_256,
        SAMPLE_512,
        SAMPLE_1024,
    };

    enum class ConversionTime : uint8_t {
        TIME_140_uS = 0,
        TIME_204_uS,
        TIME_332_uS,
        TIME_588_uS,
        TIME_1100_uS,
        TIME_2116_uS,
        TIME_4156_uS,
        TIME_8244_uS,
    };

    enum class AlertTriggerMask : uint16_t {
        SHUNT_OVER_VOLTAGE = (1 << 15),
        SHUNT_UNDER_VOLTAGE = (1 << 14),
        BUS_OVER_VOLTAGE = (1 << 13),
        BUS_UNDER_VOLTAGE = (1 << 12),
        POWER_OVER_LIMIT = (1 << 11),
        CONVERSION_READY = (1 << 10),
        ALERT_FUNCTION_FLAG = (1 << 4),
        CONVERSION_READY_FLAG = (1 << 3),
        MATH_OVERFLOW_FLAG = (1 << 2),
        ALERT_POLARITY = (1 << 1),
        ALERT_LATCH_ENABLE = (1 << 0),
        ERROR = 0,
    };

protected:
    enum class Const : uint16_t ;
    enum class Register : uint8_t;
    enum class ConfigMask : uint16_t ;
    enum class ConfigOffset : uint8_t ;
    uint16_t Current_LSB_uA;            // uA/bit

public:
    std::expected<void, std::runtime_error> Init(const uint32_t ShuntResistor_mOhm = 100, const uint32_t MaxCurrent_A = 1);

    // Getters
    int32_t GetShuntVoltage_uV();
    int32_t GetBusVoltage_mV();
    int32_t GetCurrent_uA();
    int32_t GetPower_uW();
    uint16_t GetConfig();
    uint16_t GetManufacturerID();
    uint16_t GetDieID();
    OperatingMode GetOperatingMode();
    AveragingMode GetAveragingMode();
    ConversionTime GetBusVoltageConversionTime();
    ConversionTime GetShuntVoltageConversionTime();
    uint16_t GetAlertTriggerMask();
    uint16_t GetAlertLimitValue();

    // Setters
    void Reset();
    // https://github.com/esphome/issues/issues/2999#issuecomment-1038148627
    // Max current is limited by the value of shunt resistor. 0.1ohm shunt can measure up to (2^15 -1)* 2.5µV / 0.1ohm = 819175µA (=0.819175A)
    void Calibrate(const uint32_t ShuntResistor_mOhm = 100, const uint32_t MaxCurrent_A = 1);
    void SetConfig(const uint16_t Config);
    void SetOperatingMode(OperatingMode Mode);
    void SetAveragingMode(AveragingMode Mode);
    void SetBusVoltageConversionTime(ConversionTime Time);
    void SetShuntVoltageConversionTime(ConversionTime Time);
    // If multiple functions are enabled, the highest significant bit position Alert Function (D15-D11) takes priority and responds to the Alert Limit Register
    void SetAlertTriggerMask(AlertTriggerMask AlertTriggerMask);
    void SetAlertLimitValue(uint16_t AlertLimitValue);

protected:
    virtual std::expected<void, std::runtime_error> I2C_Write(const Register Register, const uint16_t Value) = 0;
    virtual std::expected<uint16_t, std::runtime_error> I2C_Read(const Register Register) = 0;
};
