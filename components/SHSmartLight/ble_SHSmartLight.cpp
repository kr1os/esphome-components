#include "ble_SHSmartLight.h"

#include "CRC16.h"
#include "SHSmartLight.h"


namespace esphome {
namespace sh_smart_light {

    void ShSmartLightOutput::dump_config()
    {
        ESP_LOGCONFIG(TAG, "ShSmartLightOutput '%s'", light_state_ ? light_state_->get_name().c_str() : "");
        ESP_LOGCONFIG(TAG, "  Cold White Temperature: %f mireds", cold_white_temperature_);
        ESP_LOGCONFIG(TAG, "  Warm White Temperature: %f mireds", warm_white_temperature_);
        ESP_LOGCONFIG(TAG, "  Brightness: %f", brightness_);
        ESP_LOGCONFIG(TAG, "  Color Temp: %f", color_temperature_);
        ESP_LOGCONFIG(TAG, "  Minimum Brightness: %d", min_brightness_);
        ESP_LOGCONFIG(TAG, "  Transmission Duration: %d millis", tx_duration_);
    }

    void ShSmartLightOutput::write_state(light::LightState *state)
    {
        float color_temperature, brightness;
        state->current_values_as_ct(&color_temperature, &brightness);
        ESP_LOGI(TAG, "ShSmartLightOutput::write_state called! Requested ct: %f, wb: %f", color_temperature, brightness);

        if (!brightness)
        {
            sendAdvertisement(shsmart.getTurnOffCmd(true));
            _is_off = true;
            return;
        }

        uint8_t cti = 127 - (uint8_t)(127 * color_temperature);
        uint8_t bi = (uint8_t)(127 * brightness);

        if (cti < min_brightness_ && color_temperature > 0.0f)
        {
            cti = min_brightness_;
        }
        if (bi < min_brightness_ && brightness > 0.0f)
        {
            bi = min_brightness_;
        }

        ESP_LOGI(TAG, "ShSmartLightOutput::write_state values to set. ct: %d, wb: %d", cti, bi);

        if (_is_off)
        {
            sendAdvertisement(shsmart.getTurnOnCmd(true));
            _is_off = false;
        }

        sendAdvertisement(shsmart.getCCTCmd(bi, cti, true));
    }

    void ShSmartLightOutput::sendAdvertisement(std::string cmd)
    {
        ESP_LOGD(TAG, "sendAdvertisement called!");
        uint8_t completeRawAdvertisingData[cmd.length()] = {0};

        for (int i = 0; i < cmd.length(); i++)
        {
            completeRawAdvertisingData[i] = cmd[i];
        }

        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gap_config_adv_data_raw((uint8_t *) &completeRawAdvertisingData, sizeof(completeRawAdvertisingData)));
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gap_start_advertising(&ble_adv_params));
        delay(tx_duration_);
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gap_stop_advertising());
    }

}  // namespace sh_smart_light
}  // namespace esphome
