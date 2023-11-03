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
	
template <typename port_type, typename bval_type, const port_type port, const bval_type bval>
	
class led_template
{
	public:
	//constructor
	led_template()
	{
		/*set the pin low/disable the pin*/
		*reinterpret_cast<volatile port_type *>(port) &= (~(1U<<bval));
		
		/*set mode to output*/
		*reinterpret_cast<volatile port_type *>(gpio_mode_reg) |= (1U<< (2*bval));
	}
	
	static void toggle()
	{
		*reinterpret_cast<volatile port_type*>(port) ^= (1U<<bval);
	}
	
	static void pseudo_delay()
	{
			int i;
			volatile int counter = 0; 
			for(i=0;i<900000;i++)
				counter += i;		
	}
	
	private:
		/*the gpio mode register which needs to be configured for direction input or output*/
		static const port_type gpio_mode_reg = (port -0x14);
	
};
	
int main()
{
	RCC_AHB1ENR |= (1U<<0); /*Enable Clock to GPIOA*/
	
	const led_template<std::uint32_t,
				std::uint32_t,
				mcal::reg::gpioa,
				mcal::reg::gpio_pin5> led_on_p5;
	while(1)
	{
		led_on_p5.toggle();
		led_on_p5.pseudo_delay();
	}
				
}
