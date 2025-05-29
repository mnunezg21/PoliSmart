#include "DHT.h" // Incluye la librería del sensor DHT

// Define el pin al que está conectado el sensor DHT11
#define DHTPIN 10

// Define el tipo de sensor (en este caso, un DHT11) 
#define DHTTYPE DHT11   

// Inicializar el sensor DHT
DHT dht(DHTPIN, DHTTYPE);

#include <WiFiS3.h> // Librería para conexión WiFi (placas como Arduino Uno R4 WiFi)

char ssid[] = "Fiware_Space";          // Nombre de la red WiFi
char pass[] = "centroinnovacion";      // Contraseña de la red WiFi
int status = WL_IDLE_STATUS;           // Estado de la conexión WiFi

WiFiClient client;                     // Cliente WiFi

char server[] = "iota-ul.iotplatform.telefonica.com"; // Dirección del servidor IoT
int port = 8085;                                 // Puerto del servidor

// Define los pines para los LEDs
int ledRed = 12;    // LED rojo para temperaturas altas
int ledBlue = 11;    // LED azul para temperaturas bajas

int TRIG = 9;       // Pin de salida del sensor ultrasónico
int ECHO = 8;       // Pin de entrada del sensor ultrasónico
int ledMov = 7;     // LED indicador de detección de movimiento

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
  // Inicia la comunicación serial a 9600 baudios
  Serial.begin(9600);
  
  // Ejecuta el setup para conectarse al Wifi
  setup_wifi();
  // Configura los pines de los LEDs como salidas
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  // Inicializacion de los piens usados para el sensor ultra de sonido
  pinMode(ledMov, OUTPUT);   // LED para detectar movimiento (por proximidad)
  pinMode(TRIG, OUTPUT);     // Pin TRIG del sensor ultrasónico como salida
  pinMode(ECHO, INPUT);      // Pin ECHO del sensor ultrasónico como entrada

  // Inicia el sensor DHT
  dht.begin();
}

void loop() {
  // Ejecuta el sensor de ultraSonido
  ultraSonido();

  // Lee la temperatura desde el sensor (en grados Celsius)
  float temperatura = dht.readTemperature();

  // Verifica si hubo un error al leer el sensor
  if (isnan(temperatura)) {
    Serial.println("Fallo al leer del sensor DHT!");
    return;
  }

  // Imprime la temperatura en el monitor serial
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" ºC");

  // Si la temperatura es menor o igual a 19 °C, enciende el LED azul
  if (temperatura <= 19) {
    digitalWrite(ledBlue, HIGH);
  } else {
    digitalWrite(ledBlue, LOW);
  }

  // Si la temperatura es mayor o igual a 34 °C, enciende el LED rojo
  if (temperatura >= 34) {
    digitalWrite(ledRed, HIGH);
  } else {
    digitalWrite(ledRed, LOW);
  }
}

void ultraSonido(){
  long duracion;
  int distancia;

  // Enviar pulso de 10 microsegundos al TRIG
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Leer pulso de retorno
  duracion = pulseIn(ECHO, HIGH);

  // Calcular distancia en cm
  distancia = duracion * 0.034 / 2;

  // Si la distancia es menor a 20 cm, se enciende el LED de movimiento
  if (distancia < 20) {
    digitalWrite(ledMov, HIGH); 
  } else {
    digitalWrite(ledMov, LOW); 
  }
  delay(1500);
}