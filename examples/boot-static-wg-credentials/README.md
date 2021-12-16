# Example to boot Soracom Arc with static WireGuard credentials

This example sketch initialize a session of Soracom Arc with a static WireGuard credentials.

## Sequence

1. Connects Wi-Fi
2. Adjusts the clock on the local machine by NTP
3. Make a WireGuard tunnel by given static WireGuard configuration
4. Soracom Arc becomes available! This example attempts to retrieve the SIM information over the metadata service

## Note

You must replace some secret information in code, like the following:

- WiFi AP SSID
- WiFi AP Password
- Device IP address that is returned from Soracom platform
- WireGuard device private key
- WireGuard server public key
- WireGuard server address and port

__NOTICE: We highly recommend you consider storing these secrets in a secure sector on your board or using the secure method.__

