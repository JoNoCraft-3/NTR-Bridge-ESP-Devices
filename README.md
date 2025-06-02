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
NO `--break-system-packages` DOES NOT BREAK YOUR SYSTEM, IN PYTHON 3.12, THEY DECIDED TO LOCK THE NORMAL INSTALLATION, SO THIS IS NEEDED TO PROCEED
