# Innovation-Project
Modern approach to a smart home with security in mind from the beginning

## Why?
For our innovation project at Birmingham City University, we want to improve upon the security in a smart home.

The new raspberry pi contains a built in wifi card which can be turned into its own hotspot. All of the devices in our smart home will 
communicate over 802.11 WLAN with the raspberry pi acting as a base station. When the system first starts the raspberry pi generates a 
cryptographically secure random SSID name and a random WPA2 key. This is so nobody not even the people who created the system know which 
network the system is running off.

When you want to connect a new device to the system a physical serial connection must be made between your device and the base station. 
Though this connection both the raspberry pi’s connection info (WPA2 key and SSID) and a random encryption key are shared. This 
encryption key is unique per each device all protects all local traffic with AES256 standard encryption. 
Our implementation should add minimal latency which means that the user is secured while maintaining the seamless implementation of a 
smart home system.
