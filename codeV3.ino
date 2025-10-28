/*
 * ECS_Dev - Emergency Communication System
 * Version: 1.0.0
 * 
 * Long-range emergency messaging using LoRa technology
 * Works without internet, cellular, or WiFi infrastructure
 */

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// LoRa Frequency - CHANGE THIS BASED ON YOUR REGION
#define LORA_FREQUENCY 915E6  // 915MHz for US, 433MHz for EU/Asia

// Pin Definitions
#define LORA_CS    5
#define LORA_RST   14
#define LORA_DIO0  26
#define OLED_SDA   21
#define OLED_SCL   22
#define BTN_UP     32
#define BTN_SELECT 33
#define BTN_DOWN   25

// Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Device Settings
#define MAX_MESSAGES 20
#define MAX_MSG_LENGTH 100
#define DEVICE_ID "ECS_" + String(ESP.getEfuseMac(), HEX).substring(0, 4)
#define SLEEP_TIMEOUT 300000 // 5 minutes

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Message Structure
struct Message {
  String sender;
  String content;
  String type; // MSG, BCST, EMRG
  unsigned long timestamp;
  bool isRead;
};

// Global Variables
Message inbox[MAX_MESSAGES];
int messageCount = 0;
int menuIndex = 0;
int inboxIndex = 0;
String currentMessage = "";
unsigned long lastActivity = 0;
bool isAsleep = false;

// Character set for message composition
const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,!?-";
int charsetIndex = 0;

enum Screen {
  SPLASH,
  MAIN_MENU,
  SEND_MESSAGE,
  INBOX,
  BROADCAST,
  SETTINGS,
  VIEWING_MESSAGE
};

Screen currentScreen = SPLASH;

void setup() {
  Serial.begin(115200);
  
  // Initialize buttons
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  
  // Initialize display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  // Show splash screen
  showSplash();
  delay(2000);
  
  // Initialize LoRa
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(LORA_FREQUENCY)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.println("LoRa Init Failed!");
    display.println("Check wiring");
    display.display();
    while (1);
  }
  
  // Configure LoRa
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setTxPower(20);
  
  // Set callback for receiving messages
  LoRa.onReceive(onReceive);
  LoRa.receive();
  
  currentScreen = MAIN_MENU;
  lastActivity = millis();
  
  Serial.println("ECS_Dev Initialized");
  Serial.println("Device ID: " + DEVICE_ID);
}

void loop() {
  // Check for sleep timeout
  if (millis() - lastActivity > SLEEP_TIMEOUT && !isAsleep) {
    enterSleep();
  }
  
  // Handle button presses
  if (digitalRead(BTN_UP) == LOW) {
    wakeUp();
    delay(200);
    handleUpButton();
  }
  
  if (digitalRead(BTN_SELECT) == LOW) {
    wakeUp();
    delay(200);
    handleSelectButton();
  }
  
  if (digitalRead(BTN_DOWN) == LOW) {
    wakeUp();
    delay(200);
    handleDownButton();
  }
  
  // Update display based on current screen
  updateDisplay();
  
  delay(10);
}

void showSplash() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println("ECS_Dev");
  display.setTextSize(1);
  display.setCursor(5, 35);
  display.println("Emergency Comms");
  display.setCursor(30, 50);
  display.println("v1.0.0");
  display.display();
}

void updateDisplay() {
  display.clearDisplay();
  
  switch(currentScreen) {
    case MAIN_MENU:
      showMainMenu();
      break;
    case SEND_MESSAGE:
      showSendMessage();
      break;
    case INBOX:
      showInbox();
      break;
    case BROADCAST:
      showBroadcast();
      break;
    case SETTINGS:
      showSettings();
      break;
    case VIEWING_MESSAGE:
      showMessage();
      break;
  }
  
  display.display();
}

void showMainMenu() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.println("=== MAIN MENU ===");
  
  String menuItems[] = {"Send Message", "Inbox (" + String(messageCount) + ")", "Broadcast", "Settings"};
  
  for (int i = 0; i < 4; i++) {
    display.setCursor(5, 15 + (i * 12));
    if (i == menuIndex) {
      display.print("> ");
    } else {
      display.print("  ");
    }
    display.println(menuItems[i]);
  }
}

void showSendMessage() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.println("=== COMPOSE ===");
  
  display.setCursor(0, 15);
  display.println("Message:");
  display.setCursor(0, 27);
  
  // Show message with word wrap
  int cursorY = 27;
  int cursorX = 0;
  for (int i = 0; i < currentMessage.length(); i++) {
    if (cursorX >= 126) {
      cursorX = 0;
      cursorY += 10;
    }
    display.setCursor(cursorX, cursorY);
    display.print(currentMessage[i]);
    cursorX += 6;
  }
  
  // Show current character selection
  display.setCursor(0, 55);
  display.print("Char: ");
  display.print(charset[charsetIndex]);
  display.print(" [SEL:Add/SEND]");
}

void showInbox() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.println("=== INBOX ===");
  
  if (messageCount == 0) {
    display.setCursor(10, 30);
    display.println("No messages");
    return;
  }
  
  // Show messages list
  int displayCount = min(messageCount, 4);
  for (int i = 0; i < displayCount; i++) {
    int msgIdx = messageCount - 1 - i - inboxIndex;
    if (msgIdx >= 0 && msgIdx < messageCount) {
      display.setCursor(5, 12 + (i * 12));
      if (i == 0) display.print("> ");
      else display.print("  ");
      
      display.print(inbox[msgIdx].sender.substring(0, 8));
      display.print(": ");
      display.print(inbox[msgIdx].content.substring(0, 8));
      if (inbox[msgIdx].content.length() > 8) display.print("...");
    }
  }
}

void showMessage() {
  int msgIdx = messageCount - 1 - inboxIndex;
  if (msgIdx < 0 || msgIdx >= messageCount) {
    currentScreen = INBOX;
    return;
  }
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.print("From: ");
  display.println(inbox[msgIdx].sender);
  
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  
  // Display message with word wrap
  int cursorY = 15;
  int cursorX = 0;
  String msg = inbox[msgIdx].content;
  
  for (int i = 0; i < msg.length(); i++) {
    if (cursorX >= 126 || msg[i] == '\n') {
      cursorX = 0;
      cursorY += 10;
      if (cursorY > 54) break;
    }
    if (msg[i] != '\n') {
      display.setCursor(cursorX, cursorY);
      display.print(msg[i]);
      cursorX += 6;
    }
  }
  
  inbox[msgIdx].isRead = true;
}

void showBroadcast() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.println("=== EMERGENCY ===");
  
  String options[] = {"Flood Alert", "Fire Alert", "Medical Help", "Danger/War", "Other"};
  
  for (int i = 0; i < 5; i++) {
    display.setCursor(5, 15 + (i * 10));
    if (i == menuIndex) {
      display.print("> ");
    } else {
      display.print("  ");
    }
    display.println(options[i]);
  }
}

void showSettings() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.println("=== SETTINGS ===");
  
  display.setCursor(0, 15);
  display.print("Device: ");
  display.println(DEVICE_ID);
  
  display.setCursor(0, 27);
  display.print("Battery: ");
  int batteryPercent = getBatteryLevel();
  display.print(batteryPercent);
  display.println("%");
  
  display.setCursor(0, 39);
  display.print("Messages: ");
  display.print(messageCount);
  display.print("/");
  display.println(MAX_MESSAGES);
  
  display.setCursor(0, 51);
  display.print("Freq: ");
  display.print(LORA_FREQUENCY / 1E6, 0);
  display.println("MHz");
}

void handleUpButton() {
  lastActivity = millis();
  
  switch(currentScreen) {
    case MAIN_MENU:
      menuIndex = (menuIndex - 1 + 4) % 4;
      break;
    case INBOX:
      if (inboxIndex > 0) inboxIndex--;
      break;
    case BROADCAST:
      menuIndex = (menuIndex - 1 + 5) % 5;
      break;
  }
}

void handleDownButton() {
  lastActivity = millis();
  
  switch(currentScreen) {
    case MAIN_MENU:
      menuIndex = (menuIndex + 1) % 4;
      break;
    case SEND_MESSAGE:
      charsetIndex = (charsetIndex + 1) % strlen(charset);
      break;
    case INBOX:
      if (inboxIndex < messageCount - 1) inboxIndex++;
      break;
    case BROADCAST:
      menuIndex = (menuIndex + 1) % 5;
      break;
  }
}

void handleSelectButton() {
  lastActivity = millis();
  
  switch(currentScreen) {
    case MAIN_MENU:
      switch(menuIndex) {
        case 0:
          currentScreen = SEND_MESSAGE;
          currentMessage = "";
          charsetIndex = 0;
          break;
        case 1:
          currentScreen = INBOX;
          inboxIndex = 0;
          break;
        case 2:
          currentScreen = BROADCAST;
          menuIndex = 0;
          break;
        case 3:
          currentScreen = SETTINGS;
          break;
      }
      break;
      
    case SEND_MESSAGE:
      if (currentMessage.length() < MAX_MSG_LENGTH - 5) {
        currentMessage += charset[charsetIndex];
      } else if (currentMessage.endsWith("SEND")) {
        sendMessage(currentMessage.substring(0, currentMessage.length() - 4), "MSG");
        currentScreen = MAIN_MENU;
      }
      break;
      
    case INBOX:
      currentScreen = VIEWING_MESSAGE;
      break;
      
    case VIEWING_MESSAGE:
      currentScreen = INBOX;
      break;
      
    case BROADCAST:
      String alerts[] = {"FLOOD ALERT!", "FIRE EMERGENCY!", "MEDICAL HELP NEEDED!", "DANGER! STAY SAFE!", "EMERGENCY ALERT!"};
      sendMessage(alerts[menuIndex], "EMRG");
      currentScreen = MAIN_MENU;
      break;
      
    case SETTINGS:
      currentScreen = MAIN_MENU;
      break;
  }
}

void sendMessage(String content, String type) {
  String packet = DEVICE_ID + "|" + String(millis()) + "|" + type + "|" + content;
  
  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
  
  LoRa.receive(); // Return to receive mode
  
  Serial.println("Sent: " + packet);
  
  // Show confirmation
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 25);
  display.println("Message Sent!");
  display.display();
  delay(1500);
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;
  
  String received = "";
  while (LoRa.available()) {
    received += (char)LoRa.read();
  }
  
  Serial.println("Received: " + received);
  
  // Parse message
  int firstPipe = received.indexOf('|');
  int secondPipe = received.indexOf('|', firstPipe + 1);
  int thirdPipe = received.indexOf('|', secondPipe + 1);
  
  if (firstPipe > 0 && secondPipe > 0 && thirdPipe > 0) {
    String sender = received.substring(0, firstPipe);
    String timestamp = received.substring(firstPipe + 1, secondPipe);
    String type = received.substring(secondPipe + 1, thirdPipe);
    String content = received.substring(thirdPipe + 1);
    
    // Don't store own messages
    if (sender == DEVICE_ID) return;
    
    // Add to inbox
    if (messageCount < MAX_MESSAGES) {
      inbox[messageCount].sender = sender;
      inbox[messageCount].content = content;
      inbox[messageCount].type = type;
      inbox[messageCount].timestamp = timestamp.toInt();
      inbox[messageCount].isRead = false;
      messageCount++;
    } else {
      // Shift messages and add new one
      for (int i = 0; i < MAX_MESSAGES - 1; i++) {
        inbox[i] = inbox[i + 1];
      }
      inbox[MAX_MESSAGES - 1].sender = sender;
      inbox[MAX_MESSAGES - 1].content = content;
      inbox[MAX_MESSAGES - 1].type = type;
      inbox[MAX_MESSAGES - 1].timestamp = timestamp.toInt();
      inbox[MAX_MESSAGES - 1].isRead = false;
    }
    
    // Show notification
    wakeUp();
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 20);
    display.println("New Message!");
    display.setCursor(10, 35);
    display.print("From: ");
    display.println(sender);
    display.display();
    delay(2000);
  }
}

int getBatteryLevel() {
  // Read battery voltage (this is simplified - adjust for your setup)
  int rawValue = analogRead(34); // Use ADC pin connected to battery
  // Convert to percentage (adjust these values based on your battery)
  int percentage = map(rawValue, 2400, 4095, 0, 100);
  return constrain(percentage, 0, 100);
}

void enterSleep() {
  isAsleep = true;
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 28);
  display.println("Sleeping...");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  
  // Keep LoRa in receive mode but reduce power
  Serial.println("Entering sleep mode");
}

void wakeUp() {
  if (isAsleep) {
    isAsleep = false;
    Serial.println("Waking up");
  }
  lastActivity = millis();
}
