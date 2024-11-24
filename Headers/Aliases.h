#include "./_Handle_ESP32_.hpp"
#include "./u_tools.h"

namespace Spotify = Spotify_Handle_API;
namespace ESP32 = ESP32_Handler;

std::string file_path_ini = "../Config.ini";

std::map<std::string, std::map<std::string, std::string>> data;
data = readIniFile(*file_path_ini);

#define esp32_AP_SSID data["ESP32_NET"]["AP_SSID"]
#define esp32_AP_KEY data["ESP32_NET"]["AP_KEY"]
#define esp32_STA_SSID data["ESP32_NET"]["STA_SSID"]
#define esp32_STA_KEY data["ESP32_NET"]["STA_KEY"]

// BELONGS TO MeetSpace PROJECT'S SERVER ENDPOINT
// SETUP WITH TypeScript EXPRESS
const std::string IoT_API_ENDPOINT = "http://localhost:5000/api/rail/IoT";
