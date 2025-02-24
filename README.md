# EMBARCA-TEC-ATIVIDADE-Tarefa-1---Aula-S-ncrona-27-01
atividade usando a placa BitDogLab. PDF de referencia: U4C4O12T - Tarefa WLS

OBS: estarei anexando nos arquivos o video pedido.
estou enfrentando bug com o wokwi, não consigo fazer a simulação, fiz os testes diretamente na placa.

-----------------------------------------------------------------------------------------------------------------------------


*Enunciado*
Para consolidar a compreensão dos conceitos relacionados ao uso de interrupções no
microcontrolador RP2040 e explorar as funcionalidades da placa de desenvolvimento BitDogLab, propõe-se
a seguinte tarefa prática.
Objetivos
• Compreender o funcionamento e a aplicação de interrupções em microcontroladores.
• Identificar e corrigir o fenômeno do bouncing em botões por meio de debouncing via software.
• Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812.
• Fixar o estudo do uso de resistores de pull-up internos em botões de acionamento.
• Desenvolver um projeto funcional que combine hardware e software.
Descrição do Projeto:
Neste projeto, você deverá utilizar os seguintes componentes conectados à placa BitDogLab:
• Matriz 5x5 de LEDs (endereçáveis) WS2812, conectada à GPIO 7.
• LED RGB, com os pinos conectados às GPIOs (11, 12 e 13).
• Botão A conectado à GPIO 5.
• Botão B conectado à GPIO 6.
Funcionalidades do Projeto
1. O LED vermelho do LED RGB deve piscar continuamente 5 vezes por segundo.
2. O botão A deve incrementar o número exibido na matriz de LEDs cada vez que for pressionado.
3. O botão B deve decrementar o número exibido na matriz de LEDs cada vez que for pressionado.
4. Os LEDs WS2812 devem ser usados para criar efeitos visuais representando números de 0 a 9.
• Formatação fixa: Cada número deve ser exibido na matriz em um formato fixo, como
caracteres em estilo digital (ex.: segmentos iluminados que formem o número).
• Alternativamente, é permitido utilizar um estilo criativo, desde que o número seja claramente
identificável.
Requisitos do Projeto
Para o desenvolvimento, devem ser seguidos os seguintes requisitos:
1. Uso de interrupções: Todas as funcionalidades relacionadas aos botões devem ser implementadas
utilizando rotinas de interrupção (IRQ).
2. Debouncing: É obrigatório implementar o tratamento do bouncing dos botões via software.
3. Controle de LEDs: O projeto deve incluir o uso de LEDs comuns e LEDs WS2812, demonstrando o
domínio de diferentes tipos de controle.
4. Organização do código: O código deve estar bem estruturado e comentado para facilitar o
entendimento.
Entrega
Código: O código-fonte do projeto deve ser entregue em um arquivo ou repositório, contendo todos os
arquivos necessários para sua execução. Obs: O código-fonte será avaliado pelo mentor, e caso não seja
possível compilar e executar o projeto, a nota atribuída poderá ser nula.
Vídeo de demonstração: Deve ser produzido um vídeo de até 2 minutos, no qual o aluno deve:
• Aparecer pessoalmente demonstrando o funcionamento do projeto.
• Explicar brevemente as funcionalidades implementadas.
• Mostrar o projeto em execução na placa BitDogLab.
