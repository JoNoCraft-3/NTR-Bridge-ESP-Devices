#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_wifi.h>
#include <AsyncUDP.h>

const char* default_ssid = "NTR-Bridge";
const char* default_password = "";

WebServer server(80);
AsyncUDP udp;

IPAddress apIP(199,200,4,1);
IPAddress netMsk(255,255,255,0);

const String NINTENDO_SUCCESS_HTML = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
                                     "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
                                     "<html>"
                                     "<head>"
                                     "<title>HTML Page</title>"
                                     "</head>"
                                     "<body bgcolor=\"#FFFFFF\">"
                                     "This is test.html page"
                                     "</body>"
                                     "</html>";

// --- Allgemeine HTML-Header für Konsistenz ---
String getHtmlHeader(const String& title) {
  String header = "<!DOCTYPE html>";
  header += "<html><head>";
  header += "<title>" + title + "</title>";
  header += "<meta charset=\"UTF-8\">";
  header += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  header += "<style>";
  header += "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f4f4f4; color: #333; }";
  header += "h1 { color: #0056b3; }";
  header += "a { color: #007bff; text-decoration: none; }";
  header += "a:hover { text-decoration: underline; }";
  header += "ul { list-style-type: none; padding: 0; }";
  header += "li { background-color: #fff; margin-bottom: 5px; padding: 10px; border-radius: 4px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }";
  header += "form { background-color: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); max-width: 400px; margin: 20px 0; }";
  header += "input[type=\"text\"], input[type=\"password\"] { width: calc(100% - 22px); padding: 10px; margin-bottom: 10px; border: 1px solid #ddd; border-radius: 4px; }";
  header += "input[type=\"submit\"] { background-color: #28a745; color: white; padding: 10px 15px; border: none; border-radius: 4px; cursor: pointer; }";
  header += "input[type=\"submit\"]:hover { background-color: #218838; }";
  header += "</style>";
  header += "</head><body>";
  return header;
}

void handleRoot() {
  Serial.println("Handler: handleRoot() called.");
  String html = getHtmlHeader("NTR-Bridge");
  html += "<h1>Welcome to NTR-Bridge</h1>";
  html += "<p>This is the portal of your NTR-Bridge. Here you can adjust settings and access useful apps.</p>";
  html += "<p><b>Connected Device(s):</b></p><ul>";

  wifi_sta_list_t stationList;
  tcpip_adapter_sta_list_t adapterList;
  esp_wifi_ap_get_sta_list(&stationList);
  tcpip_adapter_get_sta_list(&stationList, &adapterList);

  if (adapterList.num == 0) {
    html += "<li>No connected devices. <button onclick=\"location.reload()\">Refresh</button></li>";
  } else {
    for (int i = 0; i < adapterList.num; i++) {
      tcpip_adapter_sta_info_t station = adapterList.sta[i];
      char macStr[18];
      sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
              station.mac[0], station.mac[1], station.mac[2],
              station.mac[3], station.mac[4], station.mac[5]);
      html += "<li>MAC: ";
      html += macStr;
      html += " (IP: " + IPAddress(station.ip.addr).toString() + ")";
    }
    html += " <br>";
    html += " <li><button onclick=\"location.reload()\">Refresh</button></li>";
  }

  html += "</ul>";
  html += "<p><a href=\"/config\">Wi-Fi Settings</a></p>";
  html += "<p><a href=\"/apps\">Apps</a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleConfig() {
  Serial.println("Handler: handleConfig() aufgerufen.");
  String html = getHtmlHeader("Konfiguration");
  html += "<h1>WLAN Einstellungen</h1>";
  html += "<form action=\"/set\" method=\"POST\">";
  html += "SSID: <input type=\"text\" name=\"ssid\" placeholder=\"Neuer WLAN Name\"><br>";
  html += "Passwort: <input type=\"password\" name=\"password\" placeholder=\"Neues WLAN Passwort\"><br>";
  html += "<input type=\"submit\" value=\"Speichern\">";
  html += "</form>";
  html += "<p><a href=\"/\">Zurück zur Startseite</a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleSet() {
  Serial.println("Handler: handleSet() aufgerufen.");
  String html = getHtmlHeader("Einstellungen gespeichert");
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String newSSID = server.arg("ssid");
    String newPass = server.arg("password");
    if (newSSID.length() == 0 || newPass.length() < 8) {
      html += "<p style=\"color:red;\">Fehler: SSID darf nicht leer sein und Passwort muss mindestens 8 Zeichen haben.</p>";
      html += "<p><a href=\"/config\">Zurück zu den Einstellungen</a></p>";
    } else {
      WiFi.softAPdisconnect(true);
      delay(100);
      WiFi.softAP(newSSID.c_str(), newPass.c_str());
      Serial.println("AP Einstellungen geändert zu: " + newSSID);
      html += "<p>Einstellungen gespeichert. Neuer WLAN Name: <b>" + newSSID + "</b></p>";
      html += "<p>Das Gerät startet den Access Point neu. Bitte verbinden Sie sich erneut.</p>";
      html += "<p><a href=\"/\">Zurück zur Startseite</a></p>";
    }
  } else {
    html += "<p style=\"color:red;\">Fehler: Bitte geben Sie sowohl SSID als auch Passwort ein.</p>";
    html += "<p><a href=\"/config\">Zurück zu den Einstellungen</a></p>";
  }
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleApps() {
  Serial.println("Handler: handleApps() called.");
  String html = getHtmlHeader("Apps");
  html += "<h1>Available Apps</h1>";
  html += "<p>Choose an App:</p>";
  html += "<ul>";
  html += "<li><a href=\"/app/ip\">IP-Checker</a> - Shows your current IP Adress.</li>";
  html += "<li><a href=\"/app/debug\">Debug-Console</a> - Shows internal device information.</li>";
  html += "</ul>";
  html += "<p><a href=\"/\">Back to Home</a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleAppIP() {
  Serial.println("Handler: handleAppIP() called.");
  String html = getHtmlHeader("IP-Checker");
  html += "<h1>IP-Checker</h1>";
  html += "<p>Your IP Adress in the Bridge-Network is: <b>";
  html += server.client().remoteIP().toString();
  html += "</b></p><p><a href=\"/apps\">Back to Apps</a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleDebugData() {
  StaticJsonDocument<512> doc;
  doc["uptime"] = String(millis() / 1000.0, 2);
  doc["ram"] = ESP.getFreeHeap();
  doc["flash"] = ESP.getFlashChipSize();
  doc["cpu"] = ESP.getCpuFreqMHz();
  doc["mode"] = (WiFi.getMode() == WIFI_AP ? "AP" : "UNKNOWN");
  doc["ssid"] = WiFi.softAPSSID();
  doc["ip"] = WiFi.softAPIP().toString();

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}
void handleAppDebug() {
  Serial.println("Handler: handleAppDebug() aufgerufen.");
  String html = getHtmlHeader("Debug Console");
  html += R"rawliteral(
    <h1>Debug Console</h1>
    <ul id="debugList">
      <li>Loading...</li>
    </ul>
    <button onclick="location.reload()">Refresh</button>
    <p><a href="/apps">Back to Apps</a></p>
    <script>
      function updateDebug() {
        fetch('/debug-data')
          .then(response => response.json())
          .then(data => {
            const list = document.getElementById("debugList");
            list.innerHTML = "";  // Alte Inhalte löschen

            const entries = [
              "ESP32 Uptime: " + data.uptime + " s",
              "Free RAM: " + data.ram + " Bytes",
              "Flash Size: " + data.flash + " Bytes",
              "CPU Freq: " + data.cpu + " MHz",
              "WiFi Mode: " + data.mode,
              "AP SSID: " + data.ssid,
              "AP IP: " + data.ip
            ];

            entries.forEach(info => {
              const li = document.createElement("li");
              li.textContent = info;
              list.appendChild(li);
            });
          });
      }

      setInterval(updateDebug, 200);
    </script>
  </body></html>)rawliteral";
  server.send(200, "text/html", html);
}


void handleNintendoTest() {
  Serial.println("Handler: handleNintendoTest() aufgerufen. Sende 200 OK mit Pretendo-HTML. WIP");
  server.send(200, "text/html", NINTENDO_SUCCESS_HTML); // Nutzt die globale Konstante
}
// Liste der relevanten Nintendo-Domains, die wir umleiten wollen
// Hier ist die neue Adresse "conntest.nintendowifi.net" als expliziter Testserver wichtig
const String nintendoDomains[] = {
  "ct.nintendowifi.net",
  "nas.nintendowifi.net",
  "gamestats.gs.nintendo.com",
  "c.nintendo.net",
  "conntest.nintendowifi.net", // Die von dir gefundene Domain!
  "nintendo.com", // Für generische Erreichbarkeitstests
  // Account Server
  "conntest.pretendo.cc",
  "nasc.nintendowifi.net",
  "nasc.dev.nintendowifi.net",
  "nasc.test.nintendowifi.net",
  // eShop
  "ccs.c.shop.nintendowifi.net",
  "nus.c.shop.nintendowifi.net",
  "pls.c.shop.nintendowifi.net",
  // SpotPass
  "l-npns.app.nintendowifi.net",
  "npns-dev.c.app.nintendowifi.net",
  "nppl.c.app.nintendowifi.net",
  "npul.c.app.nintendowifi.net",
  "npvk.app.nintendowifi.net",
  // Other Servers
  "ctr-o2fgs.cdn.nintendo.net"
};
const int numNintendoDomains = sizeof(nintendoDomains) / sizeof(nintendoDomains[0]);

// Hilfsfunktion zur Prüfung, ob Host eine Nintendo-Domain ist
bool isNintendoHost(const String& host) {
  for (int i = 0; i < numNintendoDomains; ++i) {
    if (host.equalsIgnoreCase(nintendoDomains[i])) {
      return true;
    }
  }
  return false;
}

// --- DNS-Server Logik ---
void setupDNS() {
  udp.onPacket([](AsyncUDPPacket packet) {
    if (packet.length() < 12) {
      Serial.println("DNS: Paket zu klein.");
      return;
    }
    uint8_t *data = packet.data();
    uint16_t queryID = (data[0] << 8) | data[1];

    String domain = "";
    int pos = 12;
    while(data[pos] != 0 && pos < packet.length()) {
      int len = data[pos++];
      if (pos + len > packet.length()) {
        Serial.println("DNS: Paketfehler - Länge überschritten.");
        break;
      }
      for(int i = 0; i < len; i++) {
        domain += (char)data[pos++];
      }
      domain += "."; // Füge Punkt am Ende hinzu
    }
    domain.toLowerCase();

    Serial.print("DNS Anfrage von ");
    Serial.print(packet.remoteIP());
    Serial.print(" für: ");
    Serial.println(domain);

    // Prüfen, ob die Domain ntr.bridge oder eine Nintendo-Domain ist
    // Wir entfernen den letzten Punkt für den Vergleich mit isNintendoHost
    if(domain == "ntr.bridge.") {
      uint8_t resp[512];
      memcpy(resp, data, packet.length());

      resp[2] |= 0x80; resp[3] |= 0x80; // QR = 1 (Response), AA = 1, RA = 1
      resp[6] = 0; resp[7] = 1;         // Answer Count auf 1

      int lenQ = pos + 5; // Startpunkt für den Antwort-Teil nach der Frage
      int posAns = lenQ;

      resp[posAns++] = 0xC0; resp[posAns++] = 0x0C; // Pointer zum Domainnamen
      resp[posAns++] = 0x00; resp[posAns++] = 0x01; // Type A (IPv4)
      resp[posAns++] = 0x00; resp[posAns++] = 0x01; // Class IN (Internet)
      resp[posAns++] = 0x00; resp[posAns++] = 0x00;
      resp[posAns++] = 0x01; resp[posAns++] = 0x2C; // TTL (300 Sekunden)
      resp[posAns++] = 0x00; resp[posAns++] = 0x04; // Data Length (4 Bytes für IPv4)
      resp[posAns++] = apIP[0];
      resp[posAns++] = apIP[1];
      resp[posAns++] = apIP[2];
      resp[posAns++] = apIP[3];

      udp.writeTo(resp, posAns, packet.remoteIP(), packet.remotePort());
      Serial.println(" -> DNS Antwort gesendet: " + apIP.toString());
    }
    if (isNintendoHost(domain.substring(0, domain.length() - 1))) {
        Serial.println(" -> Nintendo-Domain erkannt, ok.");
        handleNintendoTest();
    } else {
        Serial.println(" -> DNS Anfrage ignoriert (nicht NTR-Bridge/Nintendo-Domain).");
    }
  });

  if(udp.listen(53)) {
    Serial.println("DNS-Server gestartet auf Port 53.");
  } else {
    Serial.println("Fehler beim Starten des DNS-Servers.");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(default_ssid, default_password);

  Serial.println("Access Point gestartet: " + String(default_ssid));
  Serial.println("AP IP: " + apIP.toString());
  Serial.println("DHCP/DNS Server sollte auf " + apIP.toString() + " lauschen.");

  // Handler für unsere Web-Oberfläche
  server.on("/config", handleConfig);
  server.on("/set", HTTP_POST, handleSet);
  server.on("/apps", handleApps);
  server.on("/app/ip", handleAppIP);
  server.on("/app/debug", handleAppDebug);
  server.on("/debug-data", HTTP_GET, handleDebugData);

  // Spezifische Handler für bekannte Nintendo-Testpfade (falls exakt diese angefragt werden)
  // Ein HTTP 204 sollte hier immer das Ziel sein.
  server.on("/connectcheck.html", handleNintendoTest);
  server.on("/success.html", handleNintendoTest);
  // Wenn der 2DS XL *wirklich* eine /test.html anfragen sollte (was unüblich ist),
  // dann sollte auch dafür eine 204-Antwort gesendet werden.
  server.on("/test.html", handleNintendoTest);

  // Universal-Handler für Root-Anfragen an den Webserver
  server.on("/", HTTP_GET, []() {
    String hostHeader = server.hostHeader();  // richtig für WebServer.h

    if (hostHeader == "199.200.4.1") {
      // HTML-Seite mit Meta-Redirect
      String redirectHtml = "<html><head><meta http-equiv=\"refresh\" content=\"0; url=http://ntr.bridge/\"></head><body>Redirecting to NTR-Bridge Portal...</body></html>";
      server.send(200, "text/html", redirectHtml);
    } else {
      Serial.print("HTTP GET '/' Anfrage empfangen. Host: ");
      Serial.println(hostHeader);
      
      if (isNintendoHost(hostHeader)) {
        handleNintendoTest();
      } else if (hostHeader.equalsIgnoreCase("ntr.bridge")) {
        handleRoot();
      } else {
        Serial.println("Unbekannter Host auf Root-Pfad, Fallback auf Portal.");
        handleRoot();
      }
    }
  });

  // Universal-Handler für alle nicht gefundenen Pfade (onNotFound)
  server.onNotFound([]() {
    String currentUri = server.uri();
    String hostHeader = server.hasHeader("Host") ? server.header("Host") : "N/A";
    Serial.print("HTTP onNotFound Anfrage empfangen. URI: ");
    Serial.print(currentUri);
    Serial.print(", Host: ");
    Serial.println(hostHeader);

    // Wenn der Host eine Nintendo-Domain ist, sende 204 No Content
    if(isNintendoHost(hostHeader)) {
        handleNintendoTest();
    } else {
        // Andernfalls, liefere das normale Portal aus
        Serial.println("Unbekannter URI/Host, Fallback auf Portal.");
        handleRoot();
    }
  });

  server.begin();
  setupDNS();
}

void loop() {
  server.handleClient();
}