#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "ws2812.pio.h"

// Definições dos pinos
#define WS2812_PIN 7 // Matriz de LEDs WS2812
#define BTN_A 5      // Botão A
#define BTN_B 6      // Botão B
#define RGB_GREEN 11   // LED RGB Vermelho
#define RGB_BLUE 12 // LED RGB Verde
#define RGB_RED 13  // LED RGB Azul

// Configurações da matriz de LEDs
#define IS_RGBW false
#define NUM_PIXELS 25 // Matriz 5x5

// Variáveis globais
volatile int numero_exibido = 0;                // Número atual exibido na matriz
volatile bool atualizar_display = true;         // Flag para atualizar a matriz
volatile uint32_t ultimo_tempo_interrupcao = 0; // Debouncing

// Padrões para os números de 0 a 9 na matriz 5x5
const uint32_t padroes_numeros[10][25] = {
    // Padrão para 0
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // Padrão para 1
    {0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0},

    // Padrão para 2
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // Padrão para 3
    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 0,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // Padrão para 4
    {0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 1, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 0, 1},

    // Padrão para 5
    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1},

    // Padrão para 6
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1},

    // Padrão para 7
    {0, 0, 0, 0, 1,
     0, 1, 0, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 0, 1, 0,
     1, 1, 1, 1, 1},

    // Padrão para 8
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // Padrão para 9
    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},
};

// Função para enviar um pixel para a matriz de LEDs
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Função para converter RGB para formato GRB usado pelo WS2812
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Função para atualizar a matriz de LEDs com o número atual
void atualizar_matriz()
{
    uint32_t cor = urgb_u32(160, 32, 240); // Cor roxa
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (padroes_numeros[numero_exibido][i])
        {
            put_pixel(cor); // Acende o LED na posição i
        }
        else
        {
            put_pixel(0); // Apaga o LED na posição i
        }
    }
}

// Função de interrupção para os botões
void interrupcao_botao(uint gpio, uint32_t eventos)
{
    uint32_t agora = to_ms_since_boot(get_absolute_time());
    if ((agora - ultimo_tempo_interrupcao) > 300)
    { // Debouncing de 300ms
        if (gpio == BTN_A)
        {
            numero_exibido = (numero_exibido + 1) % 10; // Incrementa o número
        }
        else if (gpio == BTN_B)
        {
            numero_exibido = (numero_exibido - 1 + 10) % 10; // Decrementa o número
        }
        atualizar_display = true;         // Sinaliza para atualizar a matriz
        ultimo_tempo_interrupcao = agora; // Atualiza o tempo da última interrupção
    }
}

// Função para inicializar os componentes
void inicializar_componentes()
{
    // Configura os pinos dos LEDs RGB como saída
    gpio_init(RGB_RED);
    gpio_init(RGB_GREEN);
    gpio_init(RGB_BLUE);
    gpio_set_dir(RGB_RED, GPIO_OUT);
    gpio_set_dir(RGB_GREEN, GPIO_OUT);
    gpio_set_dir(RGB_BLUE, GPIO_OUT);

    // Configura os pinos dos botões como entrada com pull-up
    gpio_init(BTN_A);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_pull_up(BTN_B);

    // Configura as interrupções para os botões
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &interrupcao_botao);
    gpio_set_irq_enabled(BTN_B, GPIO_IRQ_EDGE_FALL, true);
}

// Função principal
int main()
{
    stdio_init_all();
    printf("Iniciando projeto da matriz de LEDs...\n");

    // Inicializa a matriz de LEDs WS2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Inicializa os componentes
    inicializar_componentes();

    // Loop principal
    while (1)
    {
        // Atualiza a matriz de LEDs se necessário
        if (atualizar_display)
        {
            atualizar_matriz();
            atualizar_display = false;
        }

        // Pisca o LED vermelho a 5Hz (100ms ligado, 100ms desligado)
        static bool estado_led = false;
        gpio_put(RGB_RED, estado_led);
        gpio_put(RGB_GREEN, 0); // verde desligado
        gpio_put(RGB_BLUE, 0);  //  azul desligado
        estado_led = !estado_led;
        sleep_ms(100); // 100ms para piscar a 5Hz
    }

    return 0;
}