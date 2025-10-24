## Projeto IoT: Monitoramento de Temperatura, Umidade e Luminosidade com ESP32 e MQTT
## Descrição Geral

Este projeto demonstra a implementação prática de um sistema IoT utilizando o ESP32, sensores DHT11 (temperatura e umidade) e LDR (luminosidade).
Os dados coletados são enviados via protocolo MQTT para a nuvem, permitindo a visualização e controle em tempo real através do aplicativo MyMQTT.

## Objetivos do Projeto

Coletar dados ambientais (temperatura, umidade e luminosidade).

Enviar os dados em formato JSON via MQTT com timestamp.

Permitir comunicação bidirecional (comando via MyMQTT para o ESP32).

Armazenar e visualizar dados históricos via broker MQTT ou back-end compatível.

## Componentes Utilizados

Jumpers

ESP32

Sensor DHT11	Leitura de temperatura e umidade

Sensor LDR Leitura de luminosidade


## Bibliotecas Necessárias

Instale na Arduino IDE :

DHT sensor library (by Adafruit)

Adafruit Unified Sensor

PubSubClient

NTPClient

WiFi (inclusa com ESP32 Core)


## Estrutura da Mensagem (Payload JSON)

Exemplo enviado pelo ESP32:

{
  "temperature": 25.3,
  "humidity": 60.1,
  "luminosity": 72,
  "timestamp": "2025-10-24T13:45:30Z"
}


## Fluxo de Funcionamento

O ESP32 conecta ao Wi-Fi e ao broker MQTT.

Faz leituras periódicas do DHT11 e do LDR.

O MyMQTT recebe os dados e exibe na interface.

O usuário pode enviar comandos (ex.: ligar LED) 

O ESP32 executa o comando e responde no tópico status.


## Possíveis Expansões

Envio dos dados para o FIWARE Orion Context Broker via NGSIv2 (HTTP).

Armazenamento histórico em MongoDB.

Dashboard com Node-RED Dashboard.

Adição de sensores extras (chuva, gás, movimento, etc).

## Requisitos de Ambiente

Arduino IDE 

Core do ESP32 instalado 

Acesso à internet (para NTP e broker MQTT)

Dispositivo Android com app MyMQTT instalado

## Imagem da Solução
<img width="697" height="614" alt="image" src="https://github.com/user-attachments/assets/c8401a6b-2ba1-4899-ab32-8c0ae5ed0de0" />


## Autores

Erick Anciães

Patrick Canuto

João Pedro Mendes

João Paulo Fernandes



