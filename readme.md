# HTTPS Client ESP32 with JSON Body

A simple Arduino sketch for the ESP32 (an Espressif device) that securely connects to a remote server using Wi-Fi and TLS (HTTPS) and passed data to the host using JSON content passed in the request body. The JSON body stuff is an enhancement to the [HTTPS Client ESP32 Example](https://github.com/johnwargo/https-client-esp32) project.


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



## Background

I'm working on an Arduino project that requires the ability to connect to a remote server to retrieve some data. In today's environment where security is paramount, the server I connected to (and likely any server you connected to) secures its connection using Transport Layer Security ([TLS](https://en.wikipedia.org/wiki/Transport_Layer_Security)). TLS is used in HTTPS requests to secure the connection.

Initially, I panicked a bit thinking that using HTTPS connections on an Arduino compatible device would be painful. I knew I'd have to manage certificates in my sketch and I wasn't looking forward to embedding certificates in my code and troubleshooting connection issues.

It turns out the process is not that bad, so I decided to build a sample project and publish it. I'll publish the full description on johnwargo.com and update this with a link to the post.

**Note:** To build this example, I started with the Arduino [BasicHTTPSClient](https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient/examples/BasicHttpsClient) sample sketch. That one didn't work for me, so I had to go off on my own and write this.


## Target Server

For this sketch, I needed a simple public API to use. Since I frequently use [Bacon Ipsum](https://baconipsum.com/) to generate dummy text for my apps and it has a public (free) JSON API, I decided to use that.

When you click on this link: https://baconipsum.com/api/?type=meat-and-filler&paras=1, the Bacon Ipsum API returns a single paragraph of meat themed content. That's what I needed for this sketch, a quick API call with returned content. 

## Configuring the Sketch

So I don't include my local Wi-Fi network name (SSID) and password in the sketch, I extracted those values to a separate file called `constants.h` and omitted them from this repository. In its place is a file called `constants.h.rename` that you'll use in its place. After downloading the repository, rename the `constants.h.rename` file to `constants.h` and populate it with the information for your local Wi-Fi network.

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

with, of course, your network name and password in there except for the ones I made up.

The sketch loads the configuration settings in this line in the sketch:

```c
#include "config.h"
```

## Embedding the Certificate

There are a several articles on the Internet that describe how to use certificates in an Arduino sketch. The one I started with is [ESP32 HTTPS Requests (Arduino IDE)](https://randomnerdtutorials.com/esp32-https-requests/). If it wasn't so ridiculously full of advertisements, it would be a more useful article, but it does describe how to use a Certificate Authority Certificate in an Arduino sketch. 

I created an automated process for converting a downloaded certificate file into the code needed to use it in an Arduino Sketch, you can find the details in [Automated Public Cert to Arduino Header Conversion](https://johnwargo.com/posts/2025/public-cert-arduino/). The end result is a file in the sketch called `caCert.h` that contains the public key for the root certificate used for the Bacon Ipsum API server.

Keeping the certificate in a separate file makes your code a lot easier to read. The sketch loads it using the following:

```c
#include "cert.h"
```

## Runtime

When you compile and run the sketch with the Monitor open, you'll see something like this:

```text
Connecting to MyNetwork:
..........
Wi-Fi connected
IP address: 192.168.86.82

[HTTPS] Begin
[HTTPS] GET
[HTTPS] Response: 200
["Proident sint ullamco ham ut.  Venison capicola jerky beef short loin aliqua.  Beef ribs cupidatat magna, jerky voluptate bresaola occaecat ullamco shank proident minim fatback salami.  Occaecat spare ribs venison enim.  Ham tenderloin eu est sirloin."]

Waiting 30 seconds...
```

In this case, the sketch connects to the Bacon Ipsum API and returns a 200 response (200) and a single line of meatty content.

Every 30 seconds, the sketch goes back for more content. I could have made the sketch only connect once, but what's the fun in that. I added a longer wait than the example sketch I started with since I didn't want to send too many requests to the server and have them block me for too many requests.

***

If this code helps you, please consider buying me a coffee.

<a href="https://www.buymeacoffee.com/johnwargo" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="41" width="174"></a>