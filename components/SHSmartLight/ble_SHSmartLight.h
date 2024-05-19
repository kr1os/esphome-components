#pragma once
#include <vector>
#include <string>
#include "esphome.h"
#include "esphome/core/log.h"
#include "esphome/components/light/light_output.h"
#include <esp_gap_ble_api.h>

#include "SHSmartLight.h"

namespace esphome {
namespace sh_smart_light {

static esp_ble_adv_params_t ble_adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x20,
    .adv_type = ADV_TYPE_NONCONN_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .peer_addr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .peer_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static const char* const TAG = "ShSmartLightOutput";
class ShSmartLightOutput : public Component, public light::LightOutput, public EntityBase
{
public:
    void set_color_temperature(float color_temperature) { color_temperature_ = color_temperature; }
    void set_brightness(float brightness) { brightness_ = brightness; }
    void set_cold_white_temperature(float cold_white_temperature) { cold_white_temperature_ = cold_white_temperature; }
    void set_warm_white_temperature(float warm_white_temperature) { warm_white_temperature_ = warm_white_temperature; }
    void set_min_brightness(uint8_t min_brightness) { min_brightness_ = min_brightness; }
    void set_tx_duration(uint32_t tx_duration) { tx_duration_ = tx_duration; }
    float get_setup_priority() const override { return setup_priority::BLUETOOTH; }
    void dump_config() override;
    light::LightTraits get_traits() override
    {
        auto traits = light::LightTraits();
        traits.set_supported_color_modes(
        {light::ColorMode::COLOR_TEMPERATURE});
        traits.set_min_mireds(this->cold_white_temperature_);
        traits.set_max_mireds(this->warm_white_temperature_);
        return traits;
    }
    void write_state(light::LightState* state) override;
    void setup_state(light::LightState *state) override { this->light_state_ = state; }
    void set_control_id(std::string controlID) { this->shsmart.setControlID(controlID);}
    void set_room_id(std::string roomID) { this->shsmart.setRoomID(roomID);}
protected:
    void sendAdvertisement(std::string cmd);
    float cold_white_temperature_{167};
    float warm_white_temperature_{333};
    float color_temperature_{200};
    float brightness_{200};
    uint8_t min_brightness_ = 1;
    bool _is_off;
    uint8_t tx_count_;
    uint32_t tx_duration_ = 20;
    SHSmartLight shsmart;
    light::LightState *light_state_;
};

}  // namespace sh_smart_light
}  // namespace esphome
