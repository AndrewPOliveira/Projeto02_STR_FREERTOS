# Equipe: 

Arthur Henrique Rocha Alves     - 119211079\
José Andrew Pessoa De Oliveira  - 118210339\
Valmir Ferreira da Silva        - 119211110


# Vídeo de Demonstração
Para uma explicação detalhada do funcionamento do sistema e uma demonstração da simulação,
[Vídeo de Demonstração do Funcionamento]()

# Célula de Manufatura

<p align="justify"> O sistema é composto por um depósito de entrada geral e um depósito de saída geral, além de três máquinas, cada uma equipada com seus próprios depósitos de entrada e saída. Há ainda três robôs responsáveis pela movimentação dos itens entre os depósitos e as máquinas, operando de acordo com um conjunto específico de regras pré-definidas. A funções dos robôs são tais:


  *  R1 retira insumos do depósito de entrada da célula e coloca no 
depósito de entrada de M1; 
  *  R2 retira itens produzidos por M1 e coloca no depósito de entrada de 
M2; 
  *  R3 retira itens produzidos por M1 e coloca no depósito de entrada de 
M3; 
  *  R4 retira itens produzidos por M2 e M3 e coloca no depósito de saída da célula


<p align="justify"> Cada máquina e robô envolvidos no processo possuem restrições temporais específicas, e sua operação deve ser organizada de forma a minimizar períodos de ociosidade. Diante disso, optou-se pela aplicação do conceito de microkernel, considerando a necessidade de escalonamento eficiente e comunicação entre processos. Essa abordagem se mostra especialmente vantajosa em ambientes de manufatura com possível expansão, como a inclusão de novos robôs e máquinas, já que o microkernel oferece maior flexibilidade, modularidade e facilidade de adaptação. Para a implementação, foi utilizado o FreeRTOS, um microkernel amplamente adotado em sistemas embarcados e aplicações em tempo real.

## Ferramentas e suas versões
 * `Microsoft Windows [versão 10.0.26100.3476]` 
 * `gcc 11.2.0`
 * `FreeRTOS Kernel V11.1.0`
  

## Bibliotecas utilizadas
  * **FreRTOS.h**: Contém definições fundamentais e configurações globais do sistema. É necessário ser incluído em todos os arquivos que utilizam funcionalidades do FreeRTOS.
  * **task.h**:Fornece as funções e estruturas relacionadas ao gerenciamento de tarefas. Utilizado para criar, gerenciar e monitorar tasks além de implementar a lógica concorrente do sistema.
    * Principais funções: `xTaskCreate` e `vTaskDelay`;
  * **semphr.h**: Fornece funções para manipulação de semaforos, permitindo a sincronização de processos e threads.
    * Principais funções: `xSemaphoreCreateBinary` e `xSemaphoreGive`;
  * **unistd.h**: Permite acessar a API do sistema operacional POSIX.
    * Principais funções: `sleep` e `close`;



## Clonando um Repositório Git

Para copiar esse repositório remoto na sua máquina local, utilize o comando `git clone` com o HTTPS do repositório.

```
git clone https://github.com/AndrewPOliveira/Projeto02_STR_FREERTOS.git
```

Em seguida, para compilar o código em sua máquina acesse o diretório onde você executou o git clone e execute o seguinte comando no terminal: 
```
bash run.sh
```

## Funcionamento do Sistema

<p align="center">
  <img src=https://github.com/AndrewPOliveira/Projeto02_STR_FREERTOS/blob/db9bc4a389eef48a8d631a199edeae3eb6d4c6d0/Img/C%C3%A9lulaDeManufatura.png alt="Descrição do processo."/>
</p>

<p align="justify"> O sistema simula o funcionamento de um processo industrial de manufatura, onde há a entrada do material e a saída da manufatura, passando por três máquinas e quatro robôs. As máquinas M1 e M2 produzem um item a cada 1.5 segundos, enquanto que a M3 produz um item a cada 3 segundos. Como podemos ver na imagem acima dois robôs estão entre a M1 e a M2 e M3, mas a M1 só pode ser acessada por um robô de cada vez e os seus depósitos só podem armazenar um item por vez, a máquina devendo parar enquanto não tiver o seu depósito esvaziado. 


<p align="justify"> Os robôs necessitam de 0.5 segundos para se movimentar de um depósito para outro, com exceção do R3 que leva 0.8 de trajeto. Com esse problema foi proposto soluções de utilizando dos conceitos de microkernel, tasks, sincronização e semaforos para implementação do correta da solução para esse sistema proposto
