#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#define LED_BIT 7
#define LED_DDR DDRB
#define LED_PORT PORTB
#define BAUD_RATE(x) (F_CPU/16/(x-1)) 

#define BIT(x) (1 << (x))
#define GET_BIT(r,n) ((r >> n)&1)

void init_led();
void USART1_init(volatile uint8_t baud_rate);
void USART2_init(volatile uint8_t baud_rate);
void USART0_init(volatile uint8_t baud_rate);

void USART1_transmit(unsigned char data);
void USART0_transmit(unsigned char data);
void USART0_transmit_str(unsigned char * data);
void USART0_tr_int(unsigned char data);

int main(void) {
  init_led();
  
  USART0_init(BAUD_RATE(115200));
  
  //sei();
  while(1) {
    USART0_transmit_str("Roma, I did it!\n");
    _delay_ms(1000);
  }


  return 0;
}

void USART0_tr_int(unsigned char data) {
	






}

void init_led() {
  LED_DDR |= BIT(LED_BIT);
  LED_PORT |= BIT(LED_BIT);
}

void USART1_init(volatile uint8_t baud_rate) {
  //Setting asyncronous mode
  //UCSR1C &= (~BIT(UMSEL10)) & (~BIT(UMSEL11)); //Zeros by default

  //Setting normal mode
  //UCSR1A &= ~BIT(U2X1); //Zero by default

  //Setting baud rate
  UBRR1 = baud_rate;

  //Setting up frame format
  //Setting character size to 8
  UCSR1C |= BIT(UCSZ10) | BIT(UCSZ11);
  //UCSR1B &= ~BIT(UCSZ12); //Zero by default

  //Disabling parity bit
  //UCSR1C &= (~BIT(UPM10)) & (~BIT(UPM11)); //Zeros by default

  //One stop bit
  //UCSR1C &= (~BIT(USBS1)); // Zero by default

  //Enabling transmitter
  UCSR1B |= BIT(TXEN1);
}

void USART2_init(volatile uint8_t baud_rate) {
  //Setting baud rate
  UBRR2 = baud_rate;

  //Setting up frame format
  //Setting character size to 8
  UCSR2C |= BIT(UCSZ20) | BIT(UCSZ21);
  
  //Enabling receiver
  UCSR2B |= BIT(RXEN2);  
}

void USART0_init(volatile uint8_t baud_rate) {
  //Setting baud rate
  UBRR0 = baud_rate;

  //Setting up frame format
  //Setting character size to 8
  UCSR0C |= BIT(UCSZ00) | BIT(UCSZ01);

  //Enabling transmitter
  UCSR0B |= BIT(TXEN0);
}

void USART1_transmit(unsigned char data) {
  while(!GET_BIT(UCSR1A,UDRE1));
  UDR1 = data;  
}

void USART0_transmit(unsigned char data) {
  while(!GET_BIT(UCSR0A,UDRE0));
  UDR0 = data;
}

void USART0_transmit_str(unsigned char * data) {
  int i=0;
  size_t length = strlen(data);
  while(i++ < length) {
    USART0_transmit(data[i-1]);
  }  
}

