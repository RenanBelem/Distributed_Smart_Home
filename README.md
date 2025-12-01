## Smart Home Distribuída (IoT)

Este projeto implementa um sistema de **Casa Inteligente (Smart Home)** utilizando uma arquitetura de rede **Mestre-Escravo** com quatro microcontroladores Arduino conectados via protocolo I2C (Wire).

O objetivo é centralizar o monitoramento de sensores e o controle de atuadores de diferentes cômodos (Garagem, Jardim, Quarto) através de um Arduino Mestre com um display de informações.

### ⚙️ Arquitetura do Sistema e Componentes

O sistema utiliza **quatro microcontroladores Arduino** para as seguintes funções:

| Arduino (ID) | Localização | Sensor | Atuador/Display | Responsabilidade |
| :--- | :--- | :--- | :--- | :--- |
| **Mestre (1)** | Central | N/A | Display LCD 16x2 | Controla e monitora todos os outros Arduinos, exibindo o status no LCD. |
| **Escravo (2)** | Garagem | Sensor de Presença (PIR) | LED (Luz da Garagem) | Recebe dados do sensor PIR e liga/desliga a luz (LED) com base na presença. |
| **Escravo (3)** | Jardim | Sensor de Luz (LDR) | LED (Luz do Jardim) | Recebe dados do sensor de luz e liga/desliga a luz (LED) quando escurece. |
| **Escravo (4)** | Quarto | Sensor de Temperatura (LM35) | Motor/Ventilador (Fan) | Recebe dados do sensor de temperatura e liga/desliga o ventilador com base na temperatura (> 25°C). |


---

## 💻 Projeto Lógico e Protocolo de Comunicação

O Arduino Mestre opera por meio de uma **Máquina de Estados** que ciclicamente solicita dados de cada Escravo e, em seguida, atualiza o display LCD. Os Escravos operam em um ciclo simples de espera e envio.

### 1. Máquina de Estados do Mestre

O Mestre transiciona por 8 estados em um ciclo contínuo para gerenciar a comunicação e a interface:

* **`INICIANDO`**: Estado inicial.
* **`ATUALIZANDO_GARAGEM`**: Envia a solicitação ('G') para a Garagem.
* **`ESPERANDO_GARAGEM`**: Aguarda a resposta do sensor de Presença da Garagem.
* **`ATUALIZANDO_JARDIM`**: Envia a solicitação ('J') para o Jardim.
* **`ESPERANDO_JARDIM`**: Aguarda a resposta do sensor de Luz do Jardim.
* **`ATUALIZANDO_QUARTO`**: Envia a solicitação ('Q') para o Quarto.
* **`ESPERANDO_QUARTO`**: Aguarda a resposta do sensor de Temperatura do Quarto.
* **`ATUALIZANDO_LCD`**: Atualiza as informações no display LCD com os dados recebidos.

### 2. Máquina de Estados dos Escravos (Garagem, Jardim, Quarto)

Todos os Escravos operam em um ciclo de dois estados:

* **`ESPERANDO`** : Estado padrão, o Arduino está aguardando um comando do Mestre.
* **`ENVIANDO`** : É acionado quando o comando correspondente é recebido (e.g., 'G' para Garagem). Neste estado, o Escravo lê o sensor, constrói a mensagem e a envia de volta ao Mestre, voltando em seguida para `ESPERANDO`.

### 3. Protocolo de Comunicação (Mestre $\leftrightarrow$ Escravo)

O protocolo de comunicação é baseado em mensagens de **solicitação** e **resposta** via I2C:

| Direção | Tipo de Mensagem | Exemplo | Conteúdo |
| :--- | :--- | :--- | :--- |
| **Mestre $\rightarrow$ Escravo** | Solicitação | Caractere: `'G'`, `'J'`, ou `'Q'` | O Mestre envia um **caractere** para solicitar dados específicos da Garagem, Jardim ou Quarto. |
| **Escravo $\rightarrow$ Mestre** | Resposta | String: `"G1"`, `"J0"`, `"Q26"` | O Escravo responde com uma **String** que começa com seu ID (G, J, Q) seguido do **valor do sensor** (inteiro). |

---

## 📜 Arquivos de Código-Fonte

| Arquivo | Descrição |
| :--- | :--- |
| `Defesa e Documentação Final_master.ino` | Código do Arduino Mestre. Implementa a Máquina de Estados de 8 passos e a lógica de atualização do LCD com os dados dos sensores. |
| `Defesa e Documentação Final_garagem.ino` | Código do Arduino Escravo da Garagem. Controla o Sensor PIR (`sensor`) e a Luz da Garagem (`led`). |
| `Defesa e Documentação Final_jardim.ino` | Código do Arduino Escravo do Jardim. Controla o Sensor de Luz (`sensor`) e a Luz do Jardim (`led`). |
| `Defesa e Documentação Final_quarto.ino` | Código do Arduino Escravo do Quarto. Controla o Sensor de Temperatura (`sensor`) e o Ventilador (`fan`). |
