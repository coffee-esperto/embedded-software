# embedded-software

## Plataforma
[Utilizar IDE do Arduino para NodeMCU](https://www.filipeflop.com/blog/programar-nodemcu-com-ide-arduino/)

## Libs
- AWS-SDK-ESP8266
- WiFiManager

## Configurações

### WiFi
- Está salvo no eeprom do protótipo, não precisa esquentar a cabeça.

### AWS
- Altere a linha 17 do arquivo `~/Arduino/libraries/AWS-SDK-ESP8266/src/ESP8266AWSImplentations.cpp`, adicione a instrução `sclient.setInsecure();` abaixo da instrução `WiFiClientSecure sclient;`
- Atualize o programa com as credenciais da SDK