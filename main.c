/*
 * main.c
 *
 * Created: 20.05.2021 18:59:37
 * Author : TheDukato
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
//varible of ISR
int8_t off_on_sens_var=255;
//////////////////////////////////////////////////////////////////////////Podprogramy//////////////////////////////////////////////////////////////////////////
void delay(unsigned int t_water)
{
	volatile unsigned int i;
	volatile unsigned int j;
	for (j=0;j<t_water/9;j++)
	{
	for(i=0;i<t_water;i++);
	}
}
int check_sensor_x()
{
	//Pod³¹czenie pod -----PORTC[0-5]+ PE[2]+PE[3]-----czujek maks 8
	return PINC&off_on_sens_var;
}
void on_off_sen_moi()
{
	if ( (PINB&2) == 0 )
	{
		//Dla "0", czujki wy³¹czone
		off_on_sens_var = 0;
	}
	else if ((PINB&4) == 0)
	{
		//Dla "255", czujki w³¹czone
		off_on_sens_var = 255;
	}
}
void on_pump(int8_t num_pump)
{
	//Pod³¹czenie pod -----PORTD[0-7]-----pompek maks 8
	PORTD=num_pump&15;
}
void basic()
{
	int8_t ktore_wlaczyc;
	ktore_wlaczyc=check_sensor_x();
	on_pump(ktore_wlaczyc);
	if (ktore_wlaczyc !=0)//ten if zapewnia natychmiastow¹ reakcje jesli któraœ z czujek wykryje stan "sucho"
	{
			delay(5000);//Po wejœciu do funkcji i rozpoczêciu podlewania uk³ad po podlaniu zareaguje spowrotem
	}
}
//////////////////////////////////////////////////////////////////////////Przerwania//////////////////////////////////////////////////////////////////////////
ISR(PCINT3_vect)
{
	//Dla "1" pompki w³¹czone na xxxxxxx(de_lay)
	on_pump(255);
	delay(5000);
}
//////////////////////////////////////////////////////////////////////////Koniec czêœci dot podprogramów//////////////////////////////////////////////////////////////////////////
int main(void)
{
    DDRD=0xFF;//Wyjscia uk³adu steruj¹ce pompkami
	DDRC=0x00;//Wejœcia uk³adu zbieraj¹ce sygna³ od czujek wilgoci
	DDRB=0x00;//Wejœcia uk³adu obs³uguj¹ce przerwania
	PORTB=0xFF;//W³¹czenie rezystorów podci¹gaj¹cych
	DDRE=0x00;//Wejœcia uk³adu obs³uguj¹ce przerwania
	PORTE=0xFF;//W³¹czenie rezystorów podci¹gaj¹cych
	PCICR = (1 << PCIE3);
    PCMSK3 = ( 1 << PCINT24);
	sei();
    while (1) 
    {
		basic();
		on_off_sen_moi();
    }
}

