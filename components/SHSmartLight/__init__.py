import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome import automation
from esphome.const import (
    CONF_DURATION,
    CONF_BRIGHTNESS,
    CONF_COLOR_TEMPERATURE,
    CONF_OUTPUT_ID,
    CONF_COLD_WHITE_COLOR_TEMPERATURE,
    CONF_WARM_WHITE_COLOR_TEMPERATURE,
    CONF_MIN_BRIGHTNESS,
    CONF_ID,
)

AUTO_LOAD = ["esp32_ble", "light"]
DEPENDENCIES = ["esp32"]

sh_color_temperature_ns = cg.esphome_ns.namespace('sh_smart_light')
ShSmartLightOutput = sh_color_temperature_ns.class_('ShSmartLightOutput', cg.Component, light.LightOutput)

CONFIG_SCHEMA = cv.All(
    light.RGB_LIGHT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(ShSmartLightOutput),
            cv.Optional(CONF_DURATION, default=100): cv.positive_int,
            cv.Optional(CONF_COLD_WHITE_COLOR_TEMPERATURE): cv.color_temperature,
            cv.Optional(CONF_WARM_WHITE_COLOR_TEMPERATURE): cv.color_temperature,
            cv.Required(CONF_COLOR_TEMPERATURE): cv.color_temperature,
            cv.Required(CONF_BRIGHTNESS): cv.percentage,
            cv.Optional(CONF_MIN_BRIGHTNESS, default=0x7): cv.hex_uint8_t,
        }
    ),
    cv.has_none_or_all_keys(
            [CONF_COLD_WHITE_COLOR_TEMPERATURE, CONF_WARM_WHITE_COLOR_TEMPERATURE]
    ),
    light.validate_color_temperature_channels,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await light.register_light(var, config)

    if CONF_COLD_WHITE_COLOR_TEMPERATURE in config:
        cg.add(
            var.set_cold_white_temperature(config[CONF_COLD_WHITE_COLOR_TEMPERATURE])
        )

    if CONF_WARM_WHITE_COLOR_TEMPERATURE in config:
        cg.add(
            var.set_warm_white_temperature(config[CONF_WARM_WHITE_COLOR_TEMPERATURE])
        )

    cg.add(var.set_brightness(config[CONF_BRIGHTNESS]))
    cg.add(var.set_color_temperature(config[CONF_COLOR_TEMPERATURE]))
    cg.add(var.set_min_brightness(config[CONF_MIN_BRIGHTNESS]))
    cg.add(var.set_tx_duration(config[CONF_DURATION]))
