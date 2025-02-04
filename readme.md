# HTTPS Client ESP32 with JSON Body

A simple Arduino sketch for the ESP32 (an Espressif device) that securely connects to a remote server using Wi-Fi and TLS (HTTPS) and passed data to the host using JSON content passed in the request body. The JSON body stuff is an enhancement to the [HTTPS Client ESP32](https://github.com/johnwargo/https-client-esp32) project.

## Background

In the original HTTPS CLient ESP32 project, the Arduino sketch connects to a remote host (the Bacon Ipsum service) and retrieves a paragraph of text. For this version, I created a cloud function that requires the body of the request contain a JSON object with two properties (`param1` and `param2`).

All (most) modern cloud servers require Transport Layer Security (TLS) for all connections, so the sketch must be able to connect over a secure connection (HTTPS) and pass the data to the server.

## Configuring the Sketch

So I don't include my local Wi-Fi network name (SSID) and password in the sketch, I extracted those values to a separate file called `constants.h` and omitted them from this repository. In its place is a file called `constants.h.rename` that you'll use in its place. After downloading the repository, rename the `constants.h.rename` file to `constants.h` and populate it with the information for your local Wi-Fi network.

### Network Settings

Populate the two `#define` statements with the Wi-Fi network name (`ssid`) and password:

```c
#define WIFI_SSID ""
#define WIFI_PSWD ""
```

When you're done, the file should look something like this:

```c
#define WIFI_SSID "myNetwork"
#define WIFI_PSWD "Some Complicated Wi-Fi Password"
```

With, of course, your network name and password in there except for the ones I made up.

### Parameter Array
The configuration file also defines a two-dimensional String array `parmsArray` that stores the key/value pairs passed to the host in the request body (in JSON format). 

```c
const int PARAM_ROWS = 2;
String paramsArray[PARAM_ROWS][2] = {
  { "param1", "" },
  { "param2", "" }
};
```

The host requires the JSON object to have the two listed properties (`param1` & `param2`). Right now, the values for those properties are empty, so if you run the sketch, the request will fail and you'll get a message back complaining about "missing parameters". Populate the values in the array with whatever you want and try again and it should work.

You can also increase the value in `PARAM_ROWS` and add one or more rows to the table. The server only cares about the two listed parameters, so anything else is ignored.

## Loading the Configuration Settings

The sketch loads the configuration settings in this line in the sketch:

```c
#include "config.h"
```

## Server Certificate

The project includes the server Certificate Authority (CA) certificate for the host. Without this, the HTTPS connection fails. I downloaded the certificate using [Automated Public Cert to Arduino Header Conversion](https://johnwargo.com/posts/2025/public-cert-arduino/) and converted it using the [Cert2Arduino](https://cert2arduino.netlify.app/) application. The end result is a file in the sketch called `cert.h` that contains the public key for the root certificate used for the server.

Keeping the certificate in a separate file makes your code a lot easier to read. The sketch loads it using the following:

```c
#include "cert.h"
```

## Runtime

When you compile and run the sketch with the Monitor open, you'll see something like this:

```text
Connecting to https://us-east1-jmw-demos.cloudfunctions.net/jsonbody
Building POST body
Body: { "param1": "This", "param2": "That" }

Executing POST request
HTTPS response: 200
Success
Content length: 24
Response Payload: Received "This" & "That"

Waiting 30 seconds...
```

The sketch connects to the cloud function and passes the JSON parameters in the request body. The server validates the request body and returns an HTTP code of 200 (OK) for success or 400 (Bad Request) based on the availability of the `param1` and `param2` values in the request body. 

Every 30 seconds, the sketch goes back for more content. I could have made the sketch only connect once, but what's the fun in that. Please do me a favor and don't leave the sketch running too long as I'm paying for the cloud function the sketch calls.

***

If this code helps you, please consider buying me a coffee.

<a href="https://www.buymeacoffee.com/johnwargo" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="41" width="174"></a>