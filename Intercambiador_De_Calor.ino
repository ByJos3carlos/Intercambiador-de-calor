#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Definir pines para los sensores DS18B20
#define ONE_WIRE_BUS_1 A0
#define ONE_WIRE_BUS_2 A1
#define ONE_WIRE_BUS_3 A2
#define ONE_WIRE_BUS_4 A3

// Pines de los buzzers y LEDs
#define BUZZER_1 A4
#define BUZZER_2 A5
#define LED_1 A5
#define LED_2 A5

// Configuración del bus OneWire para cada sensor DS18B20
OneWire oneWire1(ONE_WIRE_BUS_1);
OneWire oneWire2(ONE_WIRE_BUS_2);
OneWire oneWire3(ONE_WIRE_BUS_3);
OneWire oneWire4(ONE_WIRE_BUS_4);

// Configuración de los objetos DallasTemperature para cada sensor
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors3(&oneWire3);
DallasTemperature sensors4(&oneWire4);

// Configuración del LCD
LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

unsigned long previousMillis = 0; // Para el temporizador de cambio
unsigned long ledMillis = 0;      // Para el temporizador de parpadeo de LEDs
const long interval = 5000;       // Intervalo de 5 segundos para cambiar los pares de sensores
const long ledInterval = 500;     // Intervalo de 500 ms para el parpadeo de LEDs
bool showFirstPair = true;        // Flag para alternar entre pares de sensores
bool ledsOn = false;              // Estado de los LEDs (encendidos/apagados)

void setup()
{
  lcd_1.begin(16, 2); // Inicia el LCD con 16 columnas y 2 filas

  // Inicializa los sensores DS18B20
  sensors1.begin();
  sensors2.begin();
  sensors3.begin();
  sensors4.begin();

  // Configura los pines de los buzzers y LEDs como salida
  pinMode(BUZZER_1, OUTPUT);
  pinMode(BUZZER_2, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  // Asegúrate de que los buzzers y LEDs estén apagados al inicio
  digitalWrite(BUZZER_1, LOW);
  digitalWrite(BUZZER_2, LOW);
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
}

void loop()
{
  unsigned long currentMillis = millis();

  // Alternar entre los pares de sensores cada 5 segundos
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis; // Actualiza el tiempo de la última actualización
    showFirstPair = !showFirstPair; // Cambia el par de sensores a mostrar

    // Limpia el LCD antes de mostrar nuevos valores
    lcd_1.clear();

    if (showFirstPair)
    {
      // Muestra las temperaturas de los sensores 1 (A0) y 2 (A1)
      float temp1 = readTemperature(sensors1);
      float temp2 = readTemperature(sensors2);
      lcd_1.setCursor(0, 0);
      lcd_1.print("IN PVC: ");
      lcd_1.print(temp1);
      lcd_1.setCursor(0, 1);
      lcd_1.print("OUT PVC: ");
      lcd_1.print(temp2);

      // Verifica si las temperaturas están fuera del rango
      checkTemperature(temp1);
      checkTemperature(temp2);
    }
    else
    {
      // Muestra las temperaturas de los sensores 3 (A2) y 4 (A3)
      float temp3 = readTemperature(sensors3);
      float temp4 = readTemperature(sensors4);
      lcd_1.setCursor(0, 0);
      lcd_1.print("IN COBRE: ");
      lcd_1.print(temp3);
      lcd_1.setCursor(0, 1);
      lcd_1.print("OUT COBRE: ");
      lcd_1.print(temp4);

      // Verifica si las temperaturas están fuera del rango
      checkTemperature(temp3);
      checkTemperature(temp4);
    }
  }

  // Maneja el parpadeo de los LEDs cuando los buzzers están activos
  if (currentMillis - ledMillis >= ledInterval)
  {
    ledMillis = currentMillis; // Actualiza el tiempo del parpadeo de LEDs
    if (digitalRead(BUZZER_1) == HIGH) // Si los buzzers están activados
    {
      ledsOn = !ledsOn; // Cambia el estado de los LEDs
      digitalWrite(LED_1, ledsOn ? HIGH : LOW);
      digitalWrite(LED_2, ledsOn ? HIGH : LOW);
    }
    else
    {
      // Apaga los LEDs si los buzzers están apagados
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
    }
  }
}

// Función para leer la temperatura de un sensor DS18B20
float readTemperature(DallasTemperature &sensor)
{
  sensor.requestTemperatures(); // Solicita la temperatura del sensor
  float temperature = sensor.getTempCByIndex(0); // Obtiene la temperatura en grados Celsius
  return temperature;
}

// Función para verificar las temperaturas y activar el buzzer
void checkTemperature(float temperature)
{
  if (temperature <= 10 || temperature >= 70)
  {
    // Activa los buzzers si la temperatura está fuera del rango
    digitalWrite(BUZZER_1, HIGH);
    digitalWrite(BUZZER_2, HIGH);
  }
  else
  {
    // Desactiva los buzzers si la temperatura está en el rango seguro
    digitalWrite(BUZZER_1, LOW);
    digitalWrite(BUZZER_2, LOW);
  }
}
