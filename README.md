# NTR-Bridge-ESP-Devices

## 🎮 Stream your New Nintendo 3DS with low latency!

Do you love gaming on your New Nintendo 3DS and want to record your screen without lags or interruptions?  
And you have an ESP device lying around and don't know what to do with it?  
**Here is the perfect solution!**

This project improves NTR streaming by putting an ESP device into **AP mode** (Access Point mode).  
It **does not** provide an internet connection — so your NTR signal won't get disturbed by other devices using bandwidth.  
It also comes with a web interface for easy configuration.

📖 Check the [installation instructions](#installation-methods) below.

---

# 🚀 Installation Methods

## 🛠️ Preparation

1. Install [Python](https://python.org). Download the version for your OS:
   - [Windows](https://www.python.org/downloads/windows/)
   - Linux (Debian-based, usually preinstalled):  
     ```bash
     sudo apt install python3 -y
     ```
   - [macOS](https://www.python.org/downloads/macos/)

2. Install `esptool`:
   - **Windows**:  
     Open a terminal as administrator and run:  
     ```bash
     pip install esptool --break-system-packages
     ```
   - **Linux**:  
     Open a terminal and run:  
     ```bash
     sudo pip install esptool --break-system-packages
     ```

   ⚠️ **Note:** `--break-system-packages` will not break your system.  
   It is required in Python 3.12+ because of stricter package management rules.

3. Choose your installation method:
   - ✅ [GUI Method (easy)](#-installation-using-gui-method)
   - 🖥️ [Manual Terminal Method](#-installation-using-gui-method)

---

## ✅ Installation Using GUI Method

1. Plug your board into your computer.  
   Some ESP boards require a USB-to-Serial bridge (not covered here).

2. Clone or download this repository.

3. Run the installer script:
   - **Windows**:  
     ```bash
     python esp_install.py
     ```
   - **Linux**:  
     ```bash
     python3 esp_install.py
     ```

   You should now see this window:

   ![image1](https://raw.githubusercontent.com/JoNoCraft-3/NTR-Bridge-ESP-Devices/main/images/mainwindowgui.png)

4. Select your board:

   ![image2](https://raw.githubusercontent.com/JoNoCraft-3/NTR-Bridge-ESP-Devices/main/images/selectboardgui.png)

5. Click **Flash** and wait for the process to complete. Done!

➡️ Now go to [Finished Installing](#finished-installing)

---

## 🖥️ Installation Using the Terminal

1. Plug your board into your computer.  
   A USB-to-Serial bridge may be needed (not covered here).

2. Open a terminal and run the following commands:

   ```bash
   python3 -m esptool erase_flash
   python3 -m esptool write_flash 0x1000 bootloader.bin
   python3 -m esptool write_flash 0x8000 partitions.bin
   python3 -m esptool write_flash 0x10000 firmware.bin
