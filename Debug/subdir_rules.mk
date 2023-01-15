################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"D:/TI/ccs/tools/compiler/msp430-gcc-9.3.0.31_win64/bin/msp430-elf-gcc-9.3.0.exe" -c -mmcu=msp430fr2476 -mhwmult=f5series -I"D:/TI/ccs/ccs_base/msp430/include_gcc" -I"D:/TI/MSP430-SPI-Marter-Tx" -I"D:/TI/ccs/tools/compiler/msp430-gcc-9.3.0.31_win64/msp430-elf/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


