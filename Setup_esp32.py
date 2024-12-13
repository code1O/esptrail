import sys
import subprocess

esp32_RoutePorts = {
    "win32": "COM",
    "linux": "/dev/ttyUSB",
    "macos": "/dev/tty.usbserial-"
}

esp32_port = esp32_RoutePorts[sys.platform]

class flash_memory:
    def __init__(self, port_number: int = 3) -> None:
        serial_port  = esp32_port + str(port_number)
        self.instance_esptool = f"esptool --chip esp32 --port {serial_port}"
    
    def erase_flash(self) -> None:
        subprocess.Popen(f"{self.instance_esptool} erase_flash", shell=True)
    
    def write_flash(self, baud: str = "460800", firmware: str = "") -> None:
        subprocess.Popen(f"{self.instance_esptool} --baud {baud} write_flash -z 0x1000 {firmware}", shell=True)
    
    def reset(self, baud: str = "460800", firmware: str = "") -> None:
        self.erase_flash()
        self.write_flash(baud, firmware)


