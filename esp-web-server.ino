#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

WebServer server(80);

int temperature = 22;
int airHumidity = 32;
int soilHumidity = 52;
int gas = 33; 
int light = 66;

void pagehhtml() {
    String response = R"(
        <!DOCTYPE html>
        <html>
        <head>
        <title>Web Server</title>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
        </head>
        <body>
            <h1>ESP32 Server</h1>
            <p>Température: 22°C</p>
        </body>
        </html>
    )";

    server.send(200, "text/html", response);
}

void setup() {
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi ");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }

    Serial.println(" Connected!");
    Serial.println(WiFi.localIP());

    server.on("/", pagehhtml);
    server.begin();

    String url = "https://your-api.com/sensor?";
    url += "airHumidity=" + String(airHumidity);
    url += "&soilHumidity=" + String(soilHumidity);
    url += "&temperature=" + String(temperature);
    url += "&gas=" + String(gas);
    url += "&light=" + String(light);

    HTTPClient http;
    http.begin(url);

    int httpResponseCode = http.GET();
    Serial.print("HTTP Response: ");
    Serial.println(httpResponseCode);

    http.end();
}

void loop() {
    server.handleClient();
    delay(2);
}
