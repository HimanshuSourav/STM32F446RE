#include "mcal_reg.h"

////Mode Register offset 0x00
////RCC 0x4002 3800 - 0x4002 3BFF ; from memory map section
////ODR offset 0x14
////AHB1ENR = AHB1+RCC+0x30

//#define RCC_OFFSET       (0x00003800UL)
//#define RCC_BASE         (AHB1_BASE + RCC_OFFSET)

//#define RCC_AHB1ENR (*((volatile unsigned int*)0x40023830UL))
//#define GPIO_MODER (*((volatile unsigned int*)0x40020000UL))	
//#define GPIO_ODR (*((volatile unsigned int*)0x40020014UL))	
	
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

		static void set_pin_output()
		{
			*reinterpret_cast<volatile addr_type*>(address) |= static_cast<reg_type>(1U<<(val *2));			
		}
		
		static void set_pin_input()
		{
			*reinterpret_cast<volatile addr_type*>(address) |= static_cast<reg_type>(0U<<(val *2));
			*reinterpret_cast<volatile addr_type*>(address) |= static_cast<reg_type>(0U<<(1+(val *2)));
			
		}
		
		static void set_pin_analog()
		{
			*reinterpret_cast<volatile addr_type*>(address) |= static_cast<reg_type>(1U<<(val *2));
			*reinterpret_cast<volatile addr_type*>(address) |= static_cast<reg_type>(1U<<(1+(val *2)));
				
		}

		static void set_pin_alt()
		{
			*reinterpret_cast<volatile addr_type*>(address) |= static_cast<reg_type>(0U<<(val *2));
			*reinterpret_cast<volatile addr_type*>(address) |= static_cast<reg_type>(1U<<(1+(val *2)));
			
		}		
	
	
};

template <typename addr_type, typename reg_type, const addr_type port , const reg_type bpos>
class port_pin
{
	public:
		static void set_direction_output()
		{
			port_dir_type::set_pin_output();
			
		}
		static void set_direction_input()
		{
			port_dir_type::set_pin_input();
			
		}
		static void set_pin_high()
		{
			port_pin_type::set_bit();
			
		}
		static void set_pin_low()
		{
			port_pin_type::clr_bit();
			
		}
		static void read_input_value()
		{
			return port_inp_type::bit::get();
			
		}
		static void toggle()
		{
			port_pin_type::not_bit();
		}
	private:
		static const std::uint32_t odr_offset = 0x14;
		static const std::uint32_t idr_offset = 0x10;
	
		static const addr_type p_odr = port + odr_offset; 
		static const addr_type p_idr = port + idr_offset;
	
		typedef reg_access<addr_type, reg_type, p_odr, bpos> port_pin_type;
		typedef reg_access<addr_type, reg_type, p_idr, bpos> port_inp_type;
		typedef reg_access<addr_type, reg_type, port,  bpos> port_dir_type;
		
	
	
};

int main()
{
	
	const std::uint32_t abh1enr = mcal::reg::rcc::ahb1enr;
	const std::uint32_t gpioa   = mcal::reg::gpio::gpioa;
	
	typedef reg_access<std::uint32_t, std::uint32_t, abh1enr ,0U>ahb1enr_b0_type;
	
	/*Enable Clock to Port A*/
	//reg_access<std::uint32_t , 
	//						std::uint32_t,
	//						mcal::reg::rcc::ahb1enr, 
	//						(1U<<1)>::reg_or();
	ahb1enr_b0_type::set_bit();
	
	typedef port_pin<std::uint32_t, std::uint32_t, gpioa, 5U>pa5_type;
	pa5_type::set_direction_output();
	
	volatile int count=0;
	while(1)
	{
		pa5_type::toggle();
		
	//	for(int i=0;i<4800000;i++)
	//		count +=i;
		
		
	}
	
	
	
		
		return 0;
}
