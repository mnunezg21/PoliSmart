// RFID 3.3V - 3.3V | RST - 9 | GND - GND | MISO - 12 |MOSI - 11 | SCK - 13 | SDA - 10 
// RGB RED - 6 | BLUE - 5 | GREEN - 7 | GND - GND |
// BUZZER + 4 | - GND
#include <SPI.h>              // Librería para comunicación SPI (necesaria para el módulo RFID)
#include <MFRC522.h>          // Librería del lector RFID MFRC522

// Pines para el lector RFID
#define RST_PIN 9             // Pin de reset del lector RFID
#define SS_PIN 10             // Pin de selección (chip select)


#include <WiFiS3.h>           // Librería para conexión WiFi (Arduino UNO R4 WiFi u otros compatibles)

char ssid[] = "Fiware_Space";         // Nombre de la red WiFi
char pass[] = "centroinnovacion";     // Contraseña WiFi
int status = WL_IDLE_STATUS;          // Estado de la conexión WiFi

WiFiClient client;                    // Cliente WiFi para futuras conexiones

char server[] = "iota-ul.iotplatform.telefonica.com"; // Servidor de IoT (no se usa aún en el código)
int port = 8085;                      // Puerto del servidor

int redPin = 6;       // Pin del componente rojo del LED RGB
int greenPin = 7;     // Pin del componente verde del LED RGB
int bluePin = 5;      // Pin del componente azul del LED RGB
int buzzerPin = 4;    // Pin para el buzzer (emite sonidos)

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Inicializa el lector RFID

// Lista de usuarios y sus PINs (4 dígitos)
byte usuariosPermitidos[][4] = {
  {0xBD, 0xE5, 0x93, 0x81},  
  {0x12, 0x34, 0x56, 0x78}  
};
String pinesUsuarios[] = {
  "1234",  
  "4321"   
};
const int cantidadUsuarios = sizeof(usuariosPermitidos) / sizeof(usuariosPermitidos[0]);


// FUNCIÓN PARA CONECTARSE A WIFI
void setup_wifi() {
  while (!Serial);

  Serial.println("Conectando a WiFi...");

  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(1000);
    Serial.print("Intentando conectar a: ");
    Serial.println(ssid);
  }

  Serial.println("Conectado a la red WiFi");
}

void setup() {
  Serial.begin(9600);          // Inicia comunicación serial

  setup_wifi();                // Conecta al WiFi

  // Configura pines de salida
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  setColor(255, 0, 0);         // LED en rojo: estado de espera

  SPI.begin();                 // Inicia la comunicación SPI
  mfrc522.PCD_Init();          // Inicializa el lector RFID

  Serial.println("\nAcerque una tarjeta RFID al lector...");
}

void loop() {
  bool acceso=false;
  // Verifica si hay una nueva tarjeta cerca
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  byte* uid = mfrc522.uid.uidByte;  // Apunta al UID de la tarjeta

  
  // Muestra el UID por consola
  Serial.print("UID detectado: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(uid[i] < 0x10 ? " 0" : " ");
    Serial.print(uid[i], HEX);
  }
  Serial.println();
  
  // Verifica si la tarjeta es válida
  int indexUsuario = verificarUsuario(uid);

  if (indexUsuario >= 0) {
    Serial.println("✅ Usuario reconocido. Ingrese PIN (4 dígitos):");
    tone(buzzerPin, 1000);
    delay(100);
    noTone(buzzerPin);

    String pinIngresado = esperarPIN();

    Serial.print("PIN ingresado: ");
    Serial.println(pinIngresado);

    if (pinIngresado == pinesUsuarios[indexUsuario]) {
      Serial.println("✅ PIN correcto. Acceso concedido.");
      acceso=true;
      setColor(0, 255, 0);
      tone(buzzerPin, 1000);
      delay(500);
      noTone(buzzerPin);
      delay(20);
      setColor(255, 0, 0);

      Serial.println("\nAcerque una tarjeta RFID al lector...");
    } else {
      Serial.println("❌ PIN incorrecto. Acceso denegado.");
      acceso=false;
      setColor(255, 0, 0);
      tone(buzzerPin, 200);
      delay(200);
      noTone(buzzerPin);
      delay(200);
      tone(buzzerPin, 200);
      delay(200);
      noTone(buzzerPin);
      delay(20);
      Serial.println("\nAcerque una tarjeta RFID al lector...");
    }

  } else {
    Serial.println("❌ Tarjeta no registrada.");
    setColor(255, 0, 0);
    tone(buzzerPin, 200);
    delay(200);
    noTone(buzzerPin);
    delay(200);
    tone(buzzerPin, 200);
    delay(200);
    noTone(buzzerPin);
    delay(20);
    Serial.println("\nAcerque una tarjeta RFID al lector...");
  }
  httpPostmanControlAcceso(uid, acceso);
  delay(2000);
  setColor(255, 0, 0);           // LED rojo (espera)
  mfrc522.PICC_HaltA();          // Detiene la comunicación con la tarjeta
  mfrc522.PCD_StopCrypto1();     // Detiene la comunicación segura
}

int verificarUsuario(byte *uid) {
  for (int i = 0; i < cantidadUsuarios; i++) {
    bool coincide = true;
    for (int j = 0; j < 4; j++) {
      if (usuariosPermitidos[i][j] != uid[j]) {
        coincide = false;
        break;
      }
    }
    if (coincide) return i;
  }
  return -1;
}
// FUNCIÓN PARA CAMBIAR COLOR DEL LED RGB 
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

// FUNCIÓN PARA INGRESAR PIN POR SERIA
String esperarPIN() {
  String pin = "";
  while (pin.length() < 4) {
    while (Serial.available() == 0) {
    }
    int pinIntroducido = Serial.parseInt();
    pin = String(pinIntroducido);
    pin.trim(); 
  }
  return pin;
}

String uidToString(byte *uid, byte size) {
  String uidStr = "";
  for (byte i = 0; i < size; i++) {
    if (uid[i] < 0x10) uidStr += "0";  // Asegura 2 dígitos
    uidStr += String(uid[i], HEX);
  }
  uidStr.toUpperCase();  // Opcional: para mayúsculas como "BDE59381"
  return uidStr;
}


void httpPostmanControlAcceso(byte *uid, bool acceso){
  char server[] = "iota-ul.iotplatform.telefonica.com";
  int port = 8085; 

  String uidStr = uidToString(uid, 4); 
  String body = "uid|" + uidStr + "#entra|" + (acceso ? "true" : "false");
  String url = "/iot/d?k=PruebaUL&i=controlAcceso";

  if (client.connect(server, port)) {
    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: iota-ul.iotplatform.telefonica.com");
    client.println("Content-Type: text/plain");
    client.print("Content-Length: ");
    client.println(body.length());
    client.println();         
    client.println(body);
  } else {
    Serial.println("Fallo en la conexión con el servidor IoT Agent");
  }
}
