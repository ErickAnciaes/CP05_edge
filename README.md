# Projeto IoT: Monitoramento de Temperatura, Umidade e Luminosidade com ESP32 e MQTT

## Descrição Geral

Este projeto demonstra a implementação prática de um sistema IoT utilizando o **ESP32**, o sensor **DHT11** (para leitura de temperatura e umidade) e um **LDR** (para medição de luminosidade).  
Além da coleta de dados, o sistema conta com **buzzer** para alertas sonoros automáticos e controle manual via aplicativo **MyMQTT**.  

Os dados são enviados via protocolo **MQTT** para um **broker** configurado na nuvem, possibilitando o **monitoramento e controle em tempo real**.

---

## Objetivos do Projeto

- Monitorar **temperatura, umidade e luminosidade** do ambiente.  
- Enviar periodicamente os dados ao broker MQTT em **formato JSON**.  
- Receber **comandos remotos via MyMQTT**, permitindo controle direto do buzzer.  
- Gerar **alerta automático de temperatura** com buzzer (acima de 30 °C).  
- Possibilitar **comunicação bidirecional** entre o ESP32 e o aplicativo MyMQTT.  

---

## Componentes Utilizados

| Componente | Função |
|-------------|--------|
| ESP32 | Microcontrolador principal, responsável pela conexão Wi-Fi e MQTT |
| Sensor DHT11 | Medição de temperatura e umidade do ambiente |
| Sensor LDR | Medição do nível de luminosidade (entrada analógica) |
| Buzzer | Alerta sonoro automático e manual |
| Jumpers | Conexões entre sensores e o ESP32 |
| Protoboard | Montagem do circuito |

---

## Ligações dos Componentes

| Componente | Pino ESP32 |
|-------------|------------|
| DHT11 | GPIO 4 |
| LDR | GPIO 34 |
| Buzzer | GPIO 12 |

---

## Bibliotecas Necessárias

Instale na **Arduino IDE** (menu: *Sketch → Incluir Biblioteca → Gerenciar Bibliotecas...*):

- `DHT sensor library for ESPx` (by **beegee_tokyo**)  
- `PubSubClient`  
- `ArduinoJson`  
- `WiFi` *(já incluída no core do ESP32)*  

---
## Projeto Online
<img width="415" height="333" alt="image" src="https://github.com/user-attachments/assets/e474a5ec-33c2-47c5-b8dc-a1d7771ab58d" />
https://wokwi.com/projects/445713396446002177


##  Estrutura da Mensagem (Payload JSON)



{
  "temperature": 26.7,
  "humidity": 55.2,
  "light": 1023,
  "light_percent": 47,
  "ip": "192.168.0.105"
}

## Autores

Erick Anciães – RM 561484

Patrick Canuto – RM 563776

João Pedro Mendes – RM 558779

João Paulo Fernandes – RM 563430
