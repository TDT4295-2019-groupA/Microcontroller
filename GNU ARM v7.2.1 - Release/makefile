################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emusb/subdir.mk
-include emlib/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/EFM32GG11B/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: microcontroller.axf

# Tool invocations
microcontroller.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -T "microcontroller.ld" -Xlinker --gc-sections -Xlinker -Map="microcontroller.map" --specs=nano.specs -o microcontroller.axf "./CMSIS/EFM32GG11B/startup_efm32gg11b.o" "./CMSIS/EFM32GG11B/system_efm32gg11b.o" "./Drivers/dmactrl.o" "./Drivers/dmadrv.o" "./Drivers/em_dma.o" "./Drivers/em_ldma.o" "./Drivers/gpiointerrupt.o" "./Drivers/spidrv.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_ebi.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_lcd.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./emusb/em_usbd.o" "./emusb/em_usbdch9.o" "./emusb/em_usbdep.o" "./emusb/em_usbdint.o" "./emusb/em_usbh.o" "./emusb/em_usbhal.o" "./emusb/em_usbhep.o" "./emusb/em_usbhint.o" "./emusb/em_usbtimer.o" "./src/fpga.o" "./src/gpio.o" "./src/input.o" "./src/main.o" "./src/midi.o" "./src/spi.o" "./src/timer.o" "./src/usbhost.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: microcontroller.hex'
	arm-none-eabi-objcopy -O ihex "microcontroller.axf" "microcontroller.hex"
	@echo ' '

	@echo 'Building bin file: microcontroller.bin'
	arm-none-eabi-objcopy -O binary "microcontroller.axf" "microcontroller.bin"
	@echo ' '

	@echo 'Building s37 file: microcontroller.s37'
	arm-none-eabi-objcopy -O srec "microcontroller.axf" "microcontroller.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "microcontroller.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) microcontroller.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
