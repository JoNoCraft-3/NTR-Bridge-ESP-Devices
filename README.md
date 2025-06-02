# NTR-Bridge-ESP-Devices

## You really like gaming on your New Nintendo 3DS and you want to record your screen without lags or anyting and you have an ESP Devcice laying around and dont know what do to with it?
Here is the perfect solution for that.
This Project aims to make NTR Streaming better by putting an ESP Device into AP Mode (AccesPoint Mode). It does nHer ot provide an internet connection, so your NTR signal doesn't gets disturbed by other devices that consume content over the Internet. It also comes with a web interface.

Here are the [installation instructions](#installation-methods)




# Installation Methods

### Prep. work
1. Install [Python](https://python.org). Download the executable corresponding to your OS:
- [Windows](https://www.python.org/downloads/windows/)
- Linux (Debian based, should be preinstalled): In your terminal window, type `sudo apt install python3 -y`
- [macOS](https://www.python.org/downloads/macos/)

2. Install `esptool`:
- Windows: Open a windows terminal as Administrator, then type `pip install esptool --break-system-packages`
- Linux: Open a terminal window, and type `sudo pip install esptool --break-system-packages`
<br>NO `--break-system-packages` DOES NOT BREAK YOUR SYSTEM, IN PYTHON 3.12, THEY DECIDED TO LOCK THE NORMAL INSTALLATION, SO THIS IS NEEDED TO PROCEED

3. Now choose which method you want to use:
- [(EASY) GUI Method](#installation-using-gui-method)



## Installation using GUI Method

Plug your Board into your computer. You may need a USB Serial Bridge, because not every ESP has an built in Bridge. I do not integrate the use of the Bridge here.

Clone/download this repository, and run the Pythonscript `esp_install.py`:
- Windows: Open a terminal as Administrator and type `python esp_install.py`, and press *`ENTER`*
- Linux: Open a terminal and type `python3 esp_install.py`, then press *`ENTER`*
<br>
Now you'll see this window:

![image1](https://raw.githubusercontent.com/JoNoCraft-3/NTR-Bridge-ESP-Devices/main/images/mainwindowgui.png)

Now select your board:

![image2](https://raw.githubusercontent.com/JoNoCraft-3/NTR-Bridge-ESP-Devices/main/images/selectboardgui.png)

Now, that you've selected your board, click on flash an wair for it to complete. It's finished.

Head over to [`Finished installing`](#finished-installing)



## Installation using the terminal

Plug your Board into your computer. You may need a USB Serial Bridge, because not every ESP has an built in Bridge. I do not integrate the use of the Bridge here.

Open a terminal:
1. Type `python3 -m esptool erase_flash`
2. Type `python3 -m esptool write_flash 0x1000 bootloader.bin`
3. Type `python3 -m esptool write_flash 0x8000 partitions.bin`
4. Type `python3 -m esptool write_flash 0x10000 firmware.bin`

Head over to [`Finished installing`](#finished-installing)



## Finished installing

Connect to the WiFi with the name `NTR-Bridge` and open a browser. There, go to `http://ntr.bridge`, and configure your Bridge like you like it.

Now, you can connect your 3DS and your PC to the network and start recording. When you connect your 3DS, the connection test will fail, but just ignore that. Once you've set the WiFi connection up, go to the home menu, open the Rosalina Menu (R+D-Pad Down+Select) and go to `System settings` and click on `Force wireless connection`. Once clicked, select the NTR-Bridge Network. Cheers!

Note: When you cut the power connection to the bridge, the password and name of the network will reset. I'm sorry this happens, but I'm not quite there yet.
