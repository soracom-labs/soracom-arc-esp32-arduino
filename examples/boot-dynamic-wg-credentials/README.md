# Example to boot Soracom Arc with dynamic WireGuard credentials

This example sketch initialize a session of Soracom Arc dynamically according to the Soracom API credentials.

## Sequence

1. Connects Wi-Fi
2. Adjusts the clock on the local machine by NTP
3. Authenticates Soracom API according to authKeyID and authKey
4. Initialize the credentials of an Arc session with onetime WireGuard credentials; onetime means the credentials are dynamic by each initialization
5. Retrieve an Arc session to configure it
6. Make a WireGuard tunnel by credentials information and session ones
7. Soracom Arc becomes available! This example attempts to retrieve the SIM information over the metadata service

## Note

You must replace some secret information in code, like the following:

- WiFi AP SSID
- WiFi AP Password
- Soracom API Key ID
- Soracom API Key
- CA Certification
- SIM ID

__NOTICE: We highly recommend you consider storing these secrets in a secure sector on your board or using the secure method.__

