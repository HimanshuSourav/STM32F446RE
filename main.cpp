/***********
#define PERIPH_BASE      (0x40000000UL)
#define AHB1_OFFSET      (0x00020000UL)
#define AHB1_BASE        (PERIPH_BASE + AHB1_OFFSET)

#define GPIOA_OFFSET     (0x00000000UL)
#define GPIOA_BASE       (AHB1_BASE + GPIOA_OFFSET)
#define RCC_OFFSET       (0x00003800UL)
#define RCC_BASE         (AHB1_BASE + RCC_OFFSET)

#define AHB1EN_R_OFFSET  (0x00000030UL)
#define RCCAHB1EN_R      (*(volatile unsigned int*)(RCC_BASE+AHB1EN_R_OFFSET))

// base address of GPIOA mode register
#define GPIOAMODE_R_OFFSET (0x0000000UL)
#define GPIOAMODE_R        (*(volatile unsigned int*)(GPIOA_BASE+GPIOAMODE_R_OFFSET))


// base address of GPIOA output data register
#define GPIOAOD_R_OFFSET   (0x14000000UL)
#define GPIOAOD_R          (*(volatile unsigned int*)(GPIOA_BASE+GPIOAOD_R_OFFSET))


// setting explicitly 1 to position 0 of 32 bit register
#define GPIOAEN          (1U<<0)


// setting explicitly 1 to position 5 of 32 bit register
#define PIN5             (1U<<5)


#define LED_PIN          PIN5



***************/

#include "mcal_reg.h"



//Mode Register offset 0x00
//RCC 0x4002 3800 - 0x4002 3BFF ; from memory map section
//ODR offset 0x14
//AHB1ENR = AHB1+RCC+0x30


#define RCC_OFFSET       (0x00003800UL)
#define RCC_BASE         (AHB1_BASE + RCC_OFFSET)

#define RCC_AHB1ENR (*((volatile unsigned int*)0x40023830UL))
#define GPIO_MODER (*((volatile unsigned int*)0x40020000UL))	
#define GPIO_ODR (*((volatile unsigned int*)0x40020014UL))	
	
class Led
{
	public:
		typedef uint32_t port_type;
		typedef uint32_t bval_type;//bit value
	
	Led(const port_type p, bval_type b):port(p),bval(b)
	{
		// c language notation
		// *((volatile uint32_t *)GPIOB) = bval;		
		/*Disable pin*/
		*reinterpret_cast<volatile uint32_t*>(port) &= ~(1U << bval);//c++ language notation 	
		/*set pin mode to output*/
		const port_type gpio_mode_reg = port - 0x14;//substracting 0x14 from mode reg addr gives addr of data register
		*reinterpret_cast<volatile port_type*>(gpio_mode_reg) |= (1U <<(bval*2));
		
	}
	
	void toggle() const //const because the function doesnt change any class parameters
	{
		*reinterpret_cast<volatile port_type*>(port) ^=(1U<<bval);
	}
	
	void glow() const
	{
		*reinterpret_cast<volatile port_type*>(port) = (1U<<bval);
	}
	
		void stopglow() const
	{
		*reinterpret_cast<volatile port_type*>(port) = ~(1U<<bval);
	}
	void pseudo_delay()
	{
			int i;
			volatile int counter = 0; 
			for(i=0;i<900000;i++)
				counter += i;		
	}
			
	
	private:
		const port_type port;
		const bval_type bval;
};
int main()
{
	RCC_AHB1ENR |= (1U<<0); /*Enable Clock to GPIOA*/
	Led led5(mcal::reg::gpioa,mcal::reg::gpio_pin5);
	
	
	while(1)
	{
		led5.toggle();
		led5.pseudo_delay();
	}
	return 0;
		
}
