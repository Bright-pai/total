################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
PID/%.o: ../PID/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"F:/TI/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/UART" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/OLED" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/DELAY" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/PID" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/TRACE" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/BSP" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/KEY" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/STP" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/JYS" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/SINCOS" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Debug" -I"F:/TI/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"F:/TI/mspm0_sdk_2_05_01_00/source" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/ENCODER" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/GANWEI" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/PID_Pack" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/STEPPER" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/MOTOR" -gdwarf-3 -MMD -MP -MF"PID/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


