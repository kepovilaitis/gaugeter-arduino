#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

SoftwareSerial _mySerial(7, 8); // RX, TX  

#define ONE_WIRE_BUS_PIN 2

OneWire _oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature _sensors(&_oneWire);

float _oilTemperatureValue = 0.00f;
float _oilPressureValue = 0.00f;
float _waterTemperatureValue = 0.00f;
float _chargeValue = 0.00f;

union { float value; byte byteValue[4]; } _oilTemperatureFloatAsBytes;
union { float value; byte byteValue[4]; } _oilPressureFloatAsBytes;
union { float value; byte byteValue[4]; } _waterTemperatureFloatAsBytes;
union { float value; byte byteValue[4]; } _chargeFloatAsBytes;
union { float value; byte byteValue[4]; } _checksumFloatAsBytes;

double _oilPressurePin = A0;  
double _chargePin = A1;

void setup() {  
  Serial.begin(9600);
  _mySerial.begin(9600);

  _sensors.begin();
}

void loop() {  
  _sensors.requestTemperatures(); 

  _oilTemperatureValue = (float)_sensors.getTempCByIndex(0);
  _oilPressureValue = analogRead(_oilPressurePin);
  _waterTemperatureValue = (float)_sensors.getTempCByIndex(1);
  _chargeValue = analogRead(_chargePin);

//  _oilPressureValue = map (_oilPressureValue, 0, 1023, 0, 7);
  _oilPressureValue = random(70) / 10.0f;

  Serial.println(_oilPressureValue);
  
  _chargeValue = map (_chargeValue, 0, 1023, 0, 145);
  _chargeValue = _chargeValue / 10;
  
  _oilTemperatureFloatAsBytes.value = _oilTemperatureValue;
  _oilPressureFloatAsBytes.value = _oilPressureValue;
  _waterTemperatureFloatAsBytes.value = _waterTemperatureValue;
  _chargeFloatAsBytes.value = _chargeValue;
  _checksumFloatAsBytes.value = _oilPressureValue + _chargeValue + _waterTemperatureValue + _oilTemperatureValue;

  if (_mySerial.available()){
    _mySerial.write("||");

    _mySerial.write(_oilTemperatureFloatAsBytes.byteValue, 4);
    _mySerial.write(_oilPressureFloatAsBytes.byteValue, 4);
    _mySerial.write(_waterTemperatureFloatAsBytes.byteValue, 4);
    _mySerial.write(_chargeFloatAsBytes.byteValue, 4);

    _mySerial.write(_checksumFloatAsBytes.byteValue, 4);
  }
  
  delay(500);
}
