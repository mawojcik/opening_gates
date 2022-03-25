#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiGratuitous.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPUpdateServer.h>

#include <ESP8266Ping.h>

#define timeToWait 1000
#define interruption 900 // +100ms (blink_led)
#define led 2

ESP8266WebServer http_server(80);
ESP8266HTTPUpdateServer update_server;

const char* ssid = "SSID";    //hidden due to security reasons
const char* password = "PASSWORD";    //hidden due to security reasons
const String authentication_code = "random_signs";    //hidden due to security reasons
const long utcOffsetInSeconds = 3600;
const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int last_ping = 0;
#define PING_INTERVAL 10000

const IPAddress remote_ip(192, 168, 10, 1);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
String last_front_gate_opening = "";
String last_garage_gate_opening = "";
String turning_on_date = "";
int front_opening_count = 0;
int garage_opening_count = 0;

typedef enum gate {
  FRONT_GATE = 13,
  GARAGE_GATE = 15,
} gate_to_open;

void blink_led() {
  digitalWrite(led, LOW);   //inverted
  wait(100);
  digitalWrite(led, HIGH);
}

void get_info_data() {
  String auth;
  int desired_number_of_arguments = 1;
  if (http_server.args() != desired_number_of_arguments) {
    http_server.send(200, "text/plain", "Wrong number of arguments");
    goto done;
  }
  if (http_server.argName(0) == "auth" && http_server.arg(0) == authentication_code) {
    send_info();
  } else if (http_server.argName(0) != "auth") {
    http_server.send(200, "text/plain", "Wrong argument");
  } else {
    http_server.send(200, "text/plain", "Wrong authentication");
  }
done:
  blink_led();
}
void get_gate_data() {
  String auth, gate;
  int desired_number_of_arguments = 2;
  if (http_server.args() != desired_number_of_arguments) {
    http_server.send(200, "text/plain", "Wrong number of arguments");
    goto done;
  }
  // Parse the URL arguments
  for (int arg_id = 0; arg_id < desired_number_of_arguments; arg_id++) {
    if (http_server.argName(arg_id) == "auth") {
      auth = http_server.arg(arg_id);
    } else if (http_server.argName(arg_id) == "open_gate") {
      gate = http_server.arg(arg_id);
    } else {
      http_server.send(200, "text/plain", "Unknown argument " + http_server.argName(arg_id));
      goto done;
    }
  }

  if (auth != authentication_code) {
    http_server.send(200, "text/plain", "Wrong Authentication");
    goto done;
  }
  if (gate == "front") {
    http_server.send(200, "text/plain", "Opening " + gate + " gate");
    open_gate(FRONT_GATE);
  } else if (gate == "garage") {
    http_server.send(200, "text/plain", "Opening " + gate + " gate");
    open_gate(GARAGE_GATE);
  } else if (gate == "both") {
    http_server.send(200, "text/plain", "Opening " + gate + " gates");
    open_gate(GARAGE_GATE);
    blink_led();
    wait(interruption);
    open_gate(FRONT_GATE);
  } else {
    http_server.send(200, "text/plain", "Wrong gate: " + gate);
  }
done:
  blink_led();
}

void open_gate(gate_to_open gate) {
  digitalWrite(gate, HIGH);
  wait(timeToWait);
  digitalWrite(gate, LOW);
  if (gate == FRONT_GATE) {
    last_front_gate_opening = get_time_and_date();
    front_opening_count++;
  } else if (gate == GARAGE_GATE) {
    last_garage_gate_opening = get_time_and_date();
    garage_opening_count++;
  }
}
void send_info() {
  String message = "";
  message += "Turned on: " + turning_on_date + "\n";
  message += "Compiled on: " __DATE__ " "  __TIME__ "\n";
  message += "Last front gate opening:  " + last_front_gate_opening + "\n";
  message += "Last garage gate opening: " + last_garage_gate_opening + "\n";
  message += "Front gate openings: " + String(front_opening_count) + "\n";
  message += "Garage gate openings: " + String(garage_opening_count) + "\n";
  http_server.send(200, "text/pain", message);
}
String get_time_and_date() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  String data = String(monthDay) + "-" +
                String(currentMonth) + "-" +
                String(currentYear) + ",  " +
                String(daysOfTheWeek[timeClient.getDay()]) + ",   " +
                String(timeClient.getFormattedTime());
  return data;
}
void wait (int ms) {
  const unsigned long savedMillis = millis();
  unsigned long currentMillis = millis();
  while (abs(currentMillis - savedMillis) <= ms) {
    currentMillis = millis();
    yield();
  }
}
void handle_not_found() {
  http_server.send(404, "text/plain", "404: Not Found");
  blink_led();
}

void setup() {
  Serial.begin(115200);
  wifi_set_sleep_type(NONE_SLEEP_T);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    wait(10);
  }
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  pinMode(FRONT_GATE, OUTPUT);
  pinMode(GARAGE_GATE, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(FRONT_GATE, LOW);
  digitalWrite(GARAGE_GATE, LOW);
  digitalWrite(led, HIGH);    //inverted

  http_server.on("/open_gate", get_gate_data);
  http_server.on("/info", get_info_data);
  http_server.onNotFound(handle_not_found);

  update_server.setup(&http_server, "login", "password");   //credentials to upload code remotely

  http_server.begin();
  timeClient.begin();
  turning_on_date = get_time_and_date();

  experimental::ESP8266WiFiGratuitous::stationKeepAliveSetIntervalMs(PING_INTERVAL);
}

void loop() {
  http_server.handleClient();
  if (millis() - last_ping > PING_INTERVAL) {
    Ping.ping(remote_ip, 1);
    last_ping = millis();
  }
}
