
/**
 * ## `_Handle_ESP32_.h`
 * Manage ESP32 to be able to do miscellaneous tasks.
 * Additionally, enhance the communication between the ESP32 and "MeetSpace" server
 * 
 * 
*/

#ifndef _Handle_ESP32_
#define _Handle_ESP32_

#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <cstdlib>
#include <HTTPClient.h>
#include <ArduinoJson.h>

template <typename _T>
_T* allocate_mem() {
  void *ptr_mem = malloc(sizeof(_T));
  if (!ptr_mem) throw std::bad_alloc();
  return static_cast<_T*>(ptr_mem);
}

template <typename _T>
void deallocate_mem(_T* ptr) {
  if (ptr == NULL) std::cout << "error, could'nt free memory for it's null value";
  free(ptr);
}

void print_response_code (int& response_code) const {
  if (!response_code > 0) Serial.println("Something went wrong!\n Code: " + String(response_code));
  Serial.println("Task successfully done!\n Code: " + String(response_code));
}

#define double_array std::map<std::string, std::map<std::string, std::string>>

void modify_array(double_array array, std::map<std::string, std::string> new_array) {
  for (const auto& category: array) {
    for (const auto& value: category.second) {
      new_array[category.first + "_" + value.first] = value.second;
    }
  }
}

//                                           SPOTIFY API SECTION
// ==========================================================================================================================================

/**
 * 
 * Manage the Spotify API to include it in the ESP32
 * 
*/
namespace Spotify_API {

  std::map<std::string, std::map<std::string, std::string>> Spotify_API_URLS;
  std::map<std::string, std::string> API_URLS;
  std::map<std::string, std::string> Spotify_Player_Commands;
  const std::string KEY_PLAYER_SPOTIFY = "https://api.spotify.com/v1/me/player/";
  const std::string KEY_ACCOUNT_SPOTIFY = "https://api.spotify.com/v1/me/";
  const std::string KEY_PLAYER_SPOTIFY_COMMANDS = "Spotify/Player?command=";
  const std::string key_audio_features = "https://api.spotify.com/v1/audio-features/";

  /** Assign the Spotify API URLS both for music player & account */
  void assign_data(){

    // TO SEND TO THE SPOTIFY API
    Spotify_API_URLS["PLAYER"]["PAUSE"] = KEY_PLAYER_SPOTIFY + "pause";
    Spotify_API_URLS["PLAYER"]["PLAY"] = KEY_PLAYER_SPOTIFY + "play";
    Spotify_API_URLS["PLAYER"]["NEXT"] = KEY_PLAYER_SPOTIFY + "next";
    Spotify_API_URLS["PLAYER"]["PREVIOUS"] = KEY_PLAYER_SPOTIFY + "previous";
    Spotify_API_URLS["PLAYER"]["CURRENTLY_PLAYING"] = KEY_PLAYER_SPOTIFY + "currently-playing";
    Spotify_API_URLS["ACCOUNT"]["FOLLOW/UNFOLLOW"] = KEY_ACCOUNT_SPOTIFY + "following";

    modify_array(Spotify_API_URLS, API_URLS);

    // TO HANDLE ON THE ESP32 ENVIRONMENT
    Spotify_Player_Commands["PAUSE"] = KEY_PLAYER_SPOTIFY_COMMANDS + "pause";
    Spotify_Player_Commands["PLAY"] = KEY_PLAYER_SPOTIFY_COMMANDS + "play";
    Spotify_Player_Commands["NEXT"] = KEY_PLAYER_SPOTIFY_COMMANDS + "next";
    Spotify_Player_Commands["PREVIOUS"] = KEY_PLAYER_SPOTIFY_COMMANDS + "previous";

  }

  class Spotify_Handler_API {
    private:
      const std::string token;
      HTTPClient& http_Server;
    public:
      Spotify_Handler_API(HTTPClient* server, const std::string& api_token) : 
      token(api_token), http_Server(*server){}

      void API_PLAYER(const char player_command[] = "PLAY") const {
        assign_data();
        this->http_Server.begin(Spotify_API_URLS["PLAYER"][player_command]);
        this->http_Server.addHeader("Authorization", "Bearer " + this->token);
        switch (player_command) {
          case "PLAY" || "PAUSE": this->http_Server.sendRequest("PUT");
          case "NEXT" || "PREVIOUS": this->http_Server.sendRequest("POST");
        };
        print_response_code(response_code)
        this->http_Server.end();
      }

      void API_FOLLOW_UNFOLLOW(const std::string account_type, const std::string ID_S, const std::string command = "FOLLOW") {
        assign_data();
        int response;
        int *response_code = &response;
        std::string url = API_URLS["ACCOUNT_FOLLOW/UNFOLLOW"] + "?type=" + account_type + "&ids=" + ID_S;

        this->http_Server.begin(url);
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

      void analysis_audio(const std::string id_track){

        std::map<int, std::string> pitch_notation_keys;

        pitch_notation_keys[0] = "G";     pitch_notation_keys[1] = "C#/Db";
        pitch_notation_keys[2] = "D";     pitch_notation_keys[3] = "D#/Eb";
        pitch_notation_keys[4] = "E";
        pitch_notation_keys[5] = "F";     pitch_notation_keys[6] = "F#/Gb";
        pitch_notation_keys[7] = "G";     pitch_notation_keys[8] = "G#/Ab";
        pitch_notation_keys[9] = "A";     pitch_notation_keys[10] = "A#/Bb";
        pitch_notation_keys[11] = "B";    

        int key_note_json, duration;
        float tempo;

        String payload = this->http_Server.getString();
        StaticJsonDocument<200> response_doc;
        deserializeJson(response_doc, payload);

        key_note_json = pitch_notation_keys[ response_doc["key"] ];
        duration = response_doc["duration_ms"];
        
      }
  };
}
// ==========================================================================================================================================

#include <ctime>

namespace ESP32_Handler {
  namespace Spotify = Spotify_API;

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
  void HandleServer_DataPosted(const char& server_endpoint, const std::string& token_spotify) {
    int response;
    int *response_status = &response;
    char command[];
    char *ptr_command = &command;

    HTTP spotify_server, local_server;
    local_server.begin(server_endpoint);
    *response_status = local_server.GET();

    Spotify::Spotify_Handler_API Spotify_Instance(*spotify_server, token_spotify);

    // Start to listening the input data at the endpoint
    if (*response_status > 0) {
      String payload = local_server.getString();
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);

      *ptr_command = doc["Command"];
    }

    switch (*ptr_command) {
      case Spotify::Spotify_Player_Commands["PLAY"]:         Spotify_Instance.API_PLAYER(); // Command = "PLAY" by default
      case Spotify::Spotify_Player_Commands["PAUSE"]:        Spotify_Instance.API_PLAYER("PAUSE");
      case Spotify::Spotify_Player_Commands["NEXT"]:         Spotify_Instance.API_PLAYER("NEXT");
      case Spotify::Spotify_Player_Commands["PREVIOUS"]:     Spotify_Instance.API_PLAYER("PREVIOUS");
    }

    spotify_server.end();
  }

  class Commands_ESP32 {
    protected:
      const char command_recv;
    public:
      Commands_ESP32(const char& command): command_recv(command){}
      virtual void execute_command(){}
      virtual void trait_handle_received_data(const auto& data){}
  };

  class Server_Commands_ESP32 : public Commands_ESP32 {
    public:
      Server_Commands_ESP32(const char& command): Commands_ESP32(command){}
      void execute_command() const override {}
      void trait_handle_received_data(const auto& data) const override {}
  };
}

#endif