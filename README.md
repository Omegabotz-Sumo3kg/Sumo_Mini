# Sumô Mini 500g
Branch "Start" para testes gerais

Este repositório tem como objetivo armazenar o código dos robôs de sumô 500g desenvolvidos pela equipe OmegaBotz.

O projeto também conta com um [Notion]() para guiar os novos integrantes dentro do desenvolvimento.

## Configuração e Instalação de libs
No arduino IDE é necessário ter instalado para compilar o código:

Na aba "Preferences", em "Additional boards manager URLs" adicionar esta URL: "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"

Instalar lib IRRemote na versão 4.2.0 (exatamente essa)

Instalar lib PS4Controller na versão 2.1 (mais recente)

Instlar lib ESP32Servo na versão 1.1.2 (mais recente)

Para instalar corretamente a lib do ps4 você precisará:
Baixar o arquivo zipado através do link: "https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbUw2TW9SYzJvbU5GZlRkWkp..."

Adicionar a lib na aba "Sketch", depois em "Include library", em seguida "ADD .ZIP library" e selecionar o arquivo zipado baixado anteriormente,

Após adicionar a lib será necessário ir até o arquivo ps4_spp dentro do arquivo zipado e comentar as quatro linhas (como está na imagem abaixo).

image

Nesse vídeo, mostra as linhas de um dos códigos da biblioteca PS4Controller que precisa comentar e na descrição o link para baixar o arquivo zipado: https://www.youtube.com/watch?v=dRysvxQfVDw
Pareamento Controle PS4
Para realizar o pareamento é necessário identificar o Mac Address do Esp32 seguindo o código abaixo: image

Após encontrar o Mac Address do Esp32, baixe o aplicativo SixaxisPairTool para alterar o Mac Address do controle de PS4, desta forma, ambos vão ter o mesmo Mac Address e vão sincronizar via Bluetooth:

SixaxisPairTool:(https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbjJUM0cyb3BTT0R2RlFadHpRaU1lSHczVmFIUXxBQ3Jtc0trRk9zMHI1eVNPYVpZTDIwcDVZT0VkNWQ0WFNUZXFpdnMzeEtVMVVBWkpOMkJXdUZ0OWZ1MkoyVWVaamdPaEhVZFJUMDVYQUx3Y0gzREVqUHR2ZWFkQXBRb0I2WVFlX3J2eUd1VE9uTEtOQ1lOQkliZw&q=https%3A%2F%2Fsixaxispairtool.software.informer.com%2Fdownload%2F&v=dRysvxQfVDw)


Dentro da Arduino IDE é necessário configurar da seguinte forma:

Em "Select Other Board and Port", selecionar: "DOIT ESP32 DEVKIT V1" image

## Histórico de versão

| **Versão** | **Autor** |  **Descrição**  | **Revisor** |  **Data**  |
|:----------:|:---------:|:---------------:|:-----------:|:----------:|
|    1.0     | Felipe M. | Commit inicial  |      -      | 02/10/2024 |
|    2.0     | Renato Jr. | Update Read me  |      -      | 16/02/2025 |

