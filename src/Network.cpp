#include <Arduino.h>
#include <WiFi.h>
#include <esp_http_server.h>

#include "Config.hpp"
#include "Motion.hpp"
#include "Network.hpp"
#include "Sensors.hpp"

namespace {
    unsigned long lastWiFiAttempt = 0;
    unsigned long lastServerAttempt = 0;
    bool wifiConnected = false;
    httpd_handle_t server = nullptr;

    esp_err_t handleHomePage(httpd_req_t* request) {
        const Sensors::Reading reading = Sensors::getReading();

        String page;
        page.reserve(768);
        page = "<!DOCTYPE html><html lang=\"pt-BR\"><head>"
               "<meta charset=\"utf-8\"><meta http-equiv=\"refresh\" content=\"";
        page += String(Config::pageRefreshSeconds);
        page += "\">"
                "<title>ESP32</title></head><body><h1>Sensores</h1>"
                "<p>Temperatura: ";
        page += String(reading.temperature, 1);
        page += " &deg;C</p><p>Umidade: ";
        page += String(reading.humidity, 1);
        page += " %</p><p>PIR: ";
        page += Motion::getStatusText(Motion::getState());
        page += "</p><h1>ESP32</h1><p>CPU: ";
        page += ESP.getChipModel();
        page += "</p><p>Freq: ";
        page += String(ESP.getCpuFreqMHz());
        page += " MHz</p><p>RAM: ";
        page += String(ESP.getFreeHeap());
        page += " B</p></body></html>";

        httpd_resp_set_type(request, "text/html; charset=utf-8");
        httpd_resp_set_hdr(request, "Cache-Control", "no-store");
        return httpd_resp_send(request, page.c_str(), page.length());
    }

    void startServer() {
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();
        config.lru_purge_enable = true;
        if (httpd_start(&server, &config) != ESP_OK) {
            server = nullptr;
            Serial.println("Falha ao iniciar servidor HTTP; nova tentativa em 5 s.");
            return;
        }

        httpd_uri_t route = {};
        route.uri = "/";
        route.method = HTTP_GET;
        route.handler = handleHomePage;
        if (httpd_register_uri_handler(server, &route) != ESP_OK) {
            httpd_stop(server);
            server = nullptr;
            Serial.println("Falha ao registrar página HTTP; nova tentativa em 5 s.");
            return;
        }

        Serial.println("Servidor HTTP pronto na porta 80.");
    }
}

void Network::begin() {
    WiFi.mode(WIFI_STA);
    if (!WiFi.setSleep(false)) {
        Serial.println("Falha ao desativar sleep do Wi-Fi.");
    }
    WiFi.setAutoReconnect(true);
    WiFi.begin(Config::wifiSSID, Config::wifiPassword);
    lastWiFiAttempt = millis();
    lastServerAttempt = millis() - Config::serverRetryInterval;
    Serial.println("Conectando ao Wi-Fi em segundo plano...");
}

void Network::update() {
    const unsigned long now = millis();
    const bool connected = WiFi.status() == WL_CONNECTED;
    if (connected != wifiConnected) {
        wifiConnected = connected;
        if (connected) {
            Serial.print("Wi-Fi conectado. Página: http://");
            Serial.print(WiFi.localIP());
            Serial.println("/");
        } else {
            Serial.println("Wi-Fi desconectado; tentando reconectar.");
        }
    }

    if (!connected && now - lastWiFiAttempt >= Config::reconnectInterval) {
        lastWiFiAttempt = now;
        WiFi.reconnect();
    }

    if (connected && server == nullptr &&
        now - lastServerAttempt >= Config::serverRetryInterval) {
        lastServerAttempt = now;
        startServer();
    }
}
