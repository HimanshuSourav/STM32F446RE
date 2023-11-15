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
template<typename register_address_type,
					typename register_value_type>
					
	
class reg_access_dynamic
{
	public:
		static void reg_set(register_address_type address, register_value_type val )
		{
			*reinterpret_cast<volatile register_address_type*>(address) = static_cast<register_value_type>(val);
		}
		
		static register_value_type reg_get(register_address_type address)
		{
			return *reinterpret_cast<volatile register_address_type*>(address);
		}
		
		static void reg_or(register_address_type address, register_value_type val)
		{
			*reinterpret_cast<volatile register_address_type*>(address) |= static_cast<register_value_type>(val);
		}
		
		static void reg_and(register_address_type address, register_value_type val)
		{
			*reinterpret_cast<volatile register_address_type*>(address) &= static_cast<register_value_type>(val);
		}
		
		static void reg_not(register_address_type address, register_value_type val)
		{
			*reinterpret_cast<volatile register_address_type*>(address) &= static_cast<register_value_type>(~val);
		}
		
		static void set_bit(register_address_type address, register_value_type val)
		{
			*reinterpret_cast<volatile register_address_type*>(address) &= static_cast<register_value_type>(1U<<val);
		}
		
		static void clr_bit(register_address_type address, register_value_type val)
		{
			*reinterpret_cast<volatile register_address_type*>(address) |= static_cast<register_value_type>(~(1U<<val));
		}
		
		static void not_bit(register_address_type address, register_value_type val)
		{
			*reinterpret_cast<volatile register_address_type*>(address) ^= static_cast<register_value_type>(1U<<val);
		}
		
		static bool get_bit(register_address_type address, register_value_type val)
		{
			return((reg_get(address)& static_cast<register_value_type>(1U<<val)) != static_cast<register_value_type>(0U));
		}
	
	
};
	
int main()
{
	
	/*Enable Clock to Port B*/
	reg_access_dynamic<std::uint32_t, std::uint32_t>::reg_or(mcal::reg::rcc::ahb1enr,( 1U<<1));
	
	/*set first 8 pins of GPIOB to high*/
	reg_access_dynamic<std::uint32_t, std::uint32_t>::reg_set(mcal::reg::gpio::cfg_regs::gpiob_cfg::gpiob_odr, 0xFF);
	
	while(1)
	{
		//toggle bit 12
		reg_access_dynamic<std::uint32_t, std::uint32_t>::not_bit(mcal::reg::gpio::cfg_regs::gpiob_cfg::gpiob_odr, 12);
		
	}
		
		return 0;
}
