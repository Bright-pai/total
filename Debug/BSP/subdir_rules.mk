################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
BSP/%.o: ../BSP/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/Temp.LAPTOP-1A77IJ2F.003/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/PWM" -I"C:/Users/Temp.LAPTOP-1A77IJ2F.003/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/UART" -I"C:/Users/Temp.LAPTOP-1A77IJ2F.003/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/OLED" -I"C:/Users/Temp.LAPTOP-1A77IJ2F.003/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/MOTOR" -I"C:/Users/Temp.LAPTOP-1A77IJ2F.003/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/DELAY" -I"C:/Users/Temp.LAPTOP-1A77IJ2F.003/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/PID" -I"C:/Users/Temp.LAPTOP-1A77IJ2F.003/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/TRACE" -I"C:/Users/Temp.LAPTOP-1A77IJ2F.003/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/BSP" -I"C:/Users/Temp.LAPTOP-1A77IJ2F.003/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang" -I"C:/Users/Temp.LAPTOP-1A77IJ2F.003/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Debug" -I"D:/CCS_20.1.0.00006_win/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"D:/CCS_20.1.0.00006_win/mspm0_sdk_2_04_00_06/source" -gdwarf-3 -MMD -MP -MF"BSP/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


