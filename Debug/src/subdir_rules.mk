################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/opt/ti/ccs900/ccs/tools/compiler/ti-cgt-msp430_18.12.1.LTS/bin/cl430" -vmsp --use_hw_mpy=16 --include_path="/opt/ti/ccs900/ccs/ccs_base/msp430/include" --include_path="../inc" --include_path="/home/delvis/workspace/ccsv9/C14_DAC_1602_KEY_LP_FILTER_MSP430F14x_CCS9_0" --include_path="/opt/ti/ccs900/ccs/tools/compiler/ti-cgt-msp430_18.12.1.LTS/include" --advice:power=all --define=__MSP430F149__ -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


