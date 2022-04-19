#pragma once
#include "MK60DZ10.h"

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))

constexpr uint32_t PDIR(uint32_t i)
{
	return GPIO_PDIR_PDI(((1) << (i & GPIO_PDIR_PDI_MASK)));
}

void SetInterrupts()
{
	PORTE->PCR[10] = PORTE->PCR[11] =PORTE->PCR[12] = PORTE->PCR[26] = PORTE->PCR[27]
		= 0b10001010000000100000011;
	NVIC_ClearPendingIRQ(PORTE_IRQn); /* Nuluj priznak preruseni od portu B */
	NVIC_EnableIRQ(PORTE_IRQn); /* Povol preruseni od portu B */
}

/* Configuration of the necessary MCU peripherals */
void SystemConfig() {

	MCG->C4 |= (MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01)); /* Nastav hodinovy podsystem */
	SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);

	/* Turn on all port clocks */
	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;


	/* Set corresponding PTA pins (column activators of 74HC154) for GPIO functionality */
	PORTA->PCR[8] = (0 | PORT_PCR_MUX(0x01)); // A0
	PORTA->PCR[10] = (0 | PORT_PCR_MUX(0x01)); // A1
	PORTA->PCR[6] = (0 | PORT_PCR_MUX(0x01)); // A2
	PORTA->PCR[11] = (0 | PORT_PCR_MUX(0x01)); // A3

	/* Set corresponding PTA pins (rows selectors of 74HC154) for GPIO functionality */
	PORTA->PCR[26] = (0 | PORT_PCR_MUX(0x01)); // R0
	PORTA->PCR[24] = (0 | PORT_PCR_MUX(0x01)); // R1
	PORTA->PCR[9] = (0 | PORT_PCR_MUX(0x01)); // R2
	PORTA->PCR[25] = (0 | PORT_PCR_MUX(0x01)); // R3
	PORTA->PCR[28] = (0 | PORT_PCR_MUX(0x01)); // R4
	PORTA->PCR[7] = (0 | PORT_PCR_MUX(0x01)); // R5
	PORTA->PCR[27] = (0 | PORT_PCR_MUX(0x01)); // R6
	PORTA->PCR[29] = (0 | PORT_PCR_MUX(0x01)); // R7

	/* Set corresponding PTE pins (output enable of 74HC154) for GPIO functionality */
	PORTE->PCR[28] = (0 | PORT_PCR_MUX(0x01)); // #EN


	/* Change corresponding PTA port pins as outputs */
	PTA->PDDR = GPIO_PDDR_PDD(0x3F000FC0);

	/* Change corresponding PTE port pins as outputs */
	PTE->PDDR = GPIO_PDDR_PDD(GPIO_PIN(28));
}

class PortA
{
public:
	static inline void OutPin(uint32_t i)
	{
		PTA->PDOR |= GPIO_PDDR_PDD(GPIO_PIN(i));
	}
	static inline void KillPin(uint32_t i)
	{
		PTA->PDOR &= ~GPIO_PDDR_PDD(GPIO_PIN(i));
	}
	static inline void SendPin(uint32_t i)
	{
		PTA->PDOR |= GPIO_PDDR_PDD(i);
	}
public:
	void operator|=(uint32_t xvalue)
	{
		value |= xvalue;
	}
	void apply()const
	{
		PTA->PDOR = value;
	}
private:
	uint32_t value = 0;
};
