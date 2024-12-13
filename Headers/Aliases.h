#include "./_Handle_ESP32_.hpp"
#include "./u_tools.h"

namespace Spotify = Spotify_API;
namespace ESP32 = ESP32_Handler;

std::string file_path_ini = "../Config.ini";

std::map<std::string, std::map<std::string, std::string>> data;
data = readIniFile(*file_path_ini);

//          ESP32 NETWORK INFORMATION
#define esp32_AP_SSID       data["ESP32_NET"]["AP_SSID"]
#define esp32_AP_KEY        data["ESP32_NET"]["AP_KEY"]
#define esp32_STA_SSID      data["ESP32_NET"]["STA_SSID"]
#define esp32_STA_KEY       data["ESP32_NET"]["STA_KEY"]


//            WEB API INFORMATION
#define Spotify_TOKEN       data["WEB_APIS"]["Spotify_TOKEN"]


//            MQTT INFORMATION
#define mqtt_port           (int)data["MQTT"]["MQTT_PORT"]
#define mqtt_password       data["MQTT"]["MQTT_PASSWORD"]
#define mqtt_username       data["MQTT"]["MQTT_USERNAME"]
#define mqtt_server         data["MQTT"]["MQTT_SERVER"]

const std::string Self_HTTP_Server = "http://localhost:5000/";


//       NETWORK MANAGEMENT
//  ===================================
// | PROTOCOL: TCP                    |
// | ADDRESS:  localhost              |
// | PORT:     50000                  |
//  ===================================

extern "C" {
    void create_server();
    void handle_client(const char* message_to_send);
    void create_threads(const void* task);
}