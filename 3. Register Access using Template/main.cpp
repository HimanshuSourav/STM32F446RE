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
	
//template <typename port_type, typename bval_type, const port_type port, const bval_type bval>
template<typename addr_type,
					typename reg_type,
					const addr_type address,
					const reg_type val>
	
class reg_access
{
	public:
		static void reg_set()
		{
			*reinterpret_cast<volatile addr_type*>(address) = static_cast<reg_type>(val);
		}
		
		static reg_type reg_get()
		{
			return *reinterpret_cast<volatile addr_type*>(address);
		}
		
		static void reg_or()
		{
			*reinterpret_cast<volatile addr_type*>(address) |= static_cast<reg_type>(val);
		}
		
		static void reg_and()
		{
			*reinterpret_cast<volatile addr_type*>(address) &= static_cast<reg_type>(val);
		}
		
		static void reg_not()
		{
			*reinterpret_cast<volatile addr_type*>(address) &= static_cast<reg_type>(~val);
		}
		
		static void set_bit()
		{
			*reinterpret_cast<volatile addr_type*>(address) &= static_cast<reg_type>(1U<<val);
		}
		
		static void clr_bit()
		{
			*reinterpret_cast<volatile addr_type*>(address) |= static_cast<reg_type>(~(1U<<val));
		}
		
		static void not_bit()
		{
			*reinterpret_cast<volatile addr_type*>(address) ^= static_cast<reg_type>(1U<<val);
		}
		
		static bool get_bit()
		{
			return((reg_get(address)& static_cast<reg_type>(1U<<val)) != static_cast<reg_type>(0U));
		}
	
	
};
	
int main()
{
	
	/*Enable Clock to Port B*/
	reg_access<std::uint32_t , 
							std::uint32_t,
							mcal::reg::rcc::ahb1enr, 
							(1U<<1)>::reg_or();
	
	/*set first 8 pins of GPIOB to high*/
	reg_access<std::uint32_t, 
							std::uint32_t,
							mcal::reg::gpio::cfg_regs::gpiob_cfg::gpiob_odr,
							0xFF>::reg_set();
	
	while(1)
	{
		reg_access<std::uint32_t, 
							std::uint32_t,
							mcal::reg::gpio::cfg_regs::gpiob_cfg::gpiob_odr,
							12>::not_bit();//toggle bit 12
		
	}
	
	
	
		
		return 0;
}
