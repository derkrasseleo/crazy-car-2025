################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
HAL/%.obj: ../HAL/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/opt/ccstudio/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted -Ooff --opt_for_speed=0 --use_hw_mpy=F5 --include_path="/opt/ccstudio/ccs/ccs_base/msp430/include" --include_path="/home/leo/OneDrive/ECE23_S3_Traussnigg/ES/Labor/crazy-car-2025/crazy_car_2025_bichl_traussnigg/HAL" --include_path="/home/leo/OneDrive/ECE23_S3_Traussnigg/ES/Labor/crazy-car-2025/crazy_car_2025_bichl_traussnigg" --include_path="/opt/ccstudio/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power=all --define=__MSP430F5335__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="HAL/$(basename $(<F)).d_raw" --obj_directory="HAL" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


