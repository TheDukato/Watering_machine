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
	//Pod��czenie pod -----PORTC[0-5]+ PE[2]+PE[3]-----czujek maks 8
	return PINC&off_on_sens_var;
}
void on_off_sen_moi()
{
	if ( (PINB&2) == 0 )
	{
		//Dla "0", czujki wy��czone
		off_on_sens_var = 0;
	}
	else if ((PINB&4) == 0)
	{
		//Dla "255", czujki w��czone
		off_on_sens_var = 255;
	}
}
void on_pump(int8_t num_pump)
{
	//Pod��czenie pod -----PORTD[0-7]-----pompek maks 8
	PORTD=num_pump&15;
}
void basic()
{
	int8_t ktore_wlaczyc;
	ktore_wlaczyc=check_sensor_x();
	on_pump(ktore_wlaczyc);
	if (ktore_wlaczyc !=0)//ten if zapewnia natychmiastow� reakcje jesli kt�ra� z czujek wykryje stan "sucho"
	{
			delay(5000);//Po wej�ciu do funkcji i rozpocz�ciu podlewania uk�ad po podlaniu zareaguje spowrotem
	}
}
//////////////////////////////////////////////////////////////////////////Przerwania//////////////////////////////////////////////////////////////////////////
ISR(PCINT3_vect)
{
	//Dla "1" pompki w��czone na xxxxxxx(de_lay)
	on_pump(255);
	delay(5000);
}
//////////////////////////////////////////////////////////////////////////Koniec cz�ci dot podprogram�w//////////////////////////////////////////////////////////////////////////
int main(void)
{
    DDRD=0xFF;//Wyjscia uk�adu steruj�ce pompkami
	DDRC=0x00;//Wej�cia uk�adu zbieraj�ce sygna� od czujek wilgoci
	DDRB=0x00;//Wej�cia uk�adu obs�uguj�ce przerwania
	PORTB=0xFF;//W��czenie rezystor�w podci�gaj�cych
	DDRE=0x00;//Wej�cia uk�adu obs�uguj�ce przerwania
	PORTE=0xFF;//W��czenie rezystor�w podci�gaj�cych
	PCICR = (1 << PCIE3);
    PCMSK3 = ( 1 << PCINT24);
	sei();
    while (1) 
    {
		basic();
		on_off_sen_moi();
    }
}

