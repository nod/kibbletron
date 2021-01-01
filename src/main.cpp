
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Redis.h>

// WiFi credentials.
#define WIFI_SSID "WIFISSID"
#define WIFI_PASS "WIFIPASS"

#define REDIS_HOST "REDIS.HOST.NAME"
#define REDIS_PORT 6379

#define REDIS_SUB "dogfood"

Servo myservo;

int servopin = 14;    // variable to store the servo position

int openangle = 40;
int closeangle = 100;
int opentime = 650;


void getIp(char *ipCstring) {
	
	IPAddress ip = WiFi.localIP();
	utoa(ip[0], ipCstring, 10); //put the first octet in the array
	for (byte octetCounter = 1; octetCounter < 4; ++octetCounter) {
		strcat(ipCstring, ".");
		char octetCstring[4];
		utoa(ip[octetCounter], octetCstring, 10);
		strcat(ipCstring, octetCstring);
  	}
}


void connectWIFI() {
  // Connect to Wifi.
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  delay(1000);
  Serial.println("");
  Serial.println("WiFi connected ");
  Serial.println(WiFi.localIP());
  Serial.println("WiFi complete");
}


void door_open_close() {
	myservo.write(openangle);
	delay(opentime);
	myservo.write(closeangle);
}


void do_redis() {
	Serial.println("");
	Serial.println("inside do_redis");
	WiFiClient redisConn;
	if (!redisConn.connect(REDIS_HOST, REDIS_PORT))
	{
		Serial.println("Failed to connect to the Redis server!");
		return;
	}
	Redis redis(redisConn);
	char ipaddr[16];
	getIp(ipaddr);
	redis.set("kibbletron", ipaddr);

	if (redis.get("dogfood") == "eat" ) {
		redis.set("dogfood", "ate");
		door_open_close();
	}
}


void setup() {
	Serial.begin(14400);
	connectWIFI();
	myservo.attach(servopin);

	door_open_close();
}


void loop() {
	do_redis();
	delay(2000);
}


