# `ESPTRAIL`
![Photo](./Assets/esp32_camera_photo.jpg)

An `ESP32` project written in C++ with integrated `MeetSpace IoT API` & `Spotify Web API`


# What can I do with it?

- Message between IoT devices
- Smart messaging (Discord, Whatsapp)

  Save the image/message to send into the data queue untill 24hr



# How to setup

## 1.- Clone the repository in your desktop

```
cd path/to/your/Desktop
git clone https://github.com/code1O/esptrail.git
```

## 2.- Open `ARDUINO IDE`

```mermaid
graph LR;

A[Start];
B[File];
C[Open];
D[sketch_esp32];

A --> B;
B --> C;

E[Desktop path];
F[sketch_esp32.ino];
G[END];

G --> F;
F --> D;
D --> E;
C --> E;


```


## 3.- Upload

Upload the script in the `ARDUINO IDE`