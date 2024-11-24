
/**
 * ## `_Handle_ESP32_.h`
 * Manage ESP32 to be able to do miscellaneous tasks.
 * Additionally, enhance the communication between the ESP32 and "MeetSpace" server
 * 
*/

#ifndef _Handle_ESP32_
#define _Handle_ESP32_

#include <iostream>
#include <map>
#include <string>
#include <HTTPClient.h>
#include <ArduinoJson.h>


void print_response_code (int& response_code) const {
  if (!response_code > 0) Serial.println("Something went wrong!\n Code: " + String(response_code));
  Serial.println("Task successfully done!\n Code: " + String(response_code));
}

//                                           SPOTIFY API SECTION
// ==========================================================================================================================================

/**
 * 
 * Manage the Spotify API to include it in the ESP32
 * 
*/
namespace Spotify_Handle_API {

  std::map<std::string, std::map<std::string, std::string>> Spotify_API_URLS;
  std::map<std::string, std::string> Spotify_API;
  std::map<std::string, std::string> Spotify_Player_Commands;
  const std::string KEY_PLAYER_SPOTIFY = "https://api.spotify.com/v1/me/player/";
  const std::string KEY_ACCOUNT_SPOTIFY = "https://api.spotify.com/v1/me/";
  const std::string KEY_PLAYER_SPOTIFY_COMMANDS = "Spotify/Player?command=";

  /** Assign the Spotify API URLS both for music player & account */
  void assign_data(){

    // TO SEND TO THE SPOTIFY API
    Spotify_API_URLS["PLAYER"]["PAUSE"] = KEY_PLAYER_SPOTIFY + "pause";
    Spotify_API_URLS["PLAYER"]["PLAY"] = KEY_PLAYER_SPOTIFY + "play";
    Spotify_API_URLS["PLAYER"]["NEXT"] = KEY_PLAYER_SPOTIFY + "next";
    Spotify_API_URLS["PLAYER"]["PREVIOUS"] = KEY_PLAYER_SPOTIFY + "previous";
    Spotify_API_URLS["PLAYER"]["CURRENTLY_PLAYING"] = KEY_PLAYER_SPOTIFY + "currently-playing";
    Spotify_API_URLS["ACCOUNT"]["FOLLOW/UNFOLLOW"] = KEY_ACCOUNT_SPOTIFY + "following";

    // TO HANDLE ON THE ESP32 ENVIRONMENT
    Spotify_Player_Commands["PAUSE"] = KEY_PLAYER_SPOTIFY + "pause";
    Spotify_Player_Commands["PLAY"] = KEY_PLAYER_SPOTIFY + "play";
    Spotify_Player_Commands["NEXT"] = KEY_PLAYER_SPOTIFY + "next";
    Spotify_Player_Commands["PREVIOUS"] = KEY_PLAYER_SPOTIFY + "previous";

    for (const auto& category_api: Spotify_API_URLS) {
      for (const auto& action_api: category_api.second) {
        Spotify_API[category_api.first + "_" + action_api.first] = action_api.second;
      }
    }
  }

  class Spotify_Handler_API {
    private:
      const std::string token;
      std::string url_from_api;
      const auto http_Server;
    public:
      Spotify_Handler_API(const auto& server, const std::string& api_token, std::string& api_url) : 
      token(api_token), url_from_api(api_url), http_Server(server){}

      void API_PLAYER(const char& player_command = "PLAY") const {
        this->http_Server.begin(this->url_from_api);
        this->http_Server.addHeader("Authorization", "Bearer " + this->token);
        switch (player_command) {
          case "PLAY" || "PAUSE": this->http_Server.sendRequest("PUT");
          case "NEXT" || "PREVIOUS": this->http_Server.sendRequest("POST");
        });
        print_response_code(response_code)
        this->http_Server.end();
      }

      void API_FOLLOW_UNFOLLOW(const std::string account_type, const std::string ID_S, const std::string command = "FOLLOW") {
        assign_data();
        int response;
        int *response_code = &response;

        if (this->url_from_api != Spotify_API_URLS["ACCOUNT"]["FOLLOW/UNFOLLOW"]) Serial.println("Not allowed API_URL!");
        std::string url = this->url_from_api + "?type=" + account_type + "&ids=" + ID_S;

        this->http_Server.begin(this->url_from_api);
        this->http_Server.addHeader("Authorization", "Bearer " + this->token);
        switch (command) {
          case "FOLLOW": 
            *response_code = this->http_Server.sendRequest("PUT");
            break;
          case "UNFOLLOW": 
            *response_code = this->http_Server.sendRequest("DELETE");
            break;
        }
        print_response_code(&response_code);
        this->http_Server.end();
      }
  };
}
// ==========================================================================================================================================

#include <ctime>

namespace ESP32_Handler {
  namespace Spotify = Spotify_Handle_API;

  std::time_t ttime = std::time(nullptr);
  std::tm* localtime = std::localtime(&ttime);
  const int hour = localtime->tm_hour;
  const int minute = localtime->tm_minute;
  const int second = localtime->tm_second;

  void wait_time_sleep(int& light_sleep_time, int& deep_sleep_time){
    bool minute_geq_light_sleep, minute_leq_deep_sleep; 
    minute_geq_light_sleep = (minute >= light_sleep_time);
    minute_leq_deep_sleep = (minute <= deep_sleep_time);

    if ( (minute >= light_sleep_time) && (minute <= deep_sleep_time) ) esp_light_sleep_start();
    else if ( minute == deep_sleep_time ) esp_deep_sleep_start();
  }

  // Manage remotetly all the data sent to the server endpoint
  // DEFAULT ENDPOINT: http://localhost:5000/api/rail/IoT/
  template <typename _TR>
  _TR HandleServer_DataPosted(const char& server_endpoint) {
    Spotify::assign_data();

    const char command[];
    const char *ptr_command = &command;

    HTTP http_server;
    // Start to listening the input data at the endpoint
    http_server.begin(server_endpoint);
    int response_status = http_server.GET();

    if (response_status > 0) {
      String payload = http_server.getString();
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);

      *ptr_command = doc["Command"];
    }

    switch (*ptr_command) {
    }

    http_server.end();
  }

  template <typename _T>

  class Commands_ESP32 {
    protected:
      const char* command_recv;
    public:
      Commands_ESP32(const char& command): command_recv(command){}
      virtual void execute_command(){}
      virtual void trait_handle_received_data(_T& data){}
  };

  template <typename _T>

  class Server_Commands_ESP32 : public Commands_ESP32 {
    public:
      Server_Commands_ESP32(const char& command): Commands_ESP32(command){}
      void execute_command() const override{}
      void trait_handle_received_data(_T& data) const override{}
  };
}

#endif