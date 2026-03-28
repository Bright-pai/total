################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MOTOR/%.o: ../MOTOR/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/Ti/CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/UART" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/OLED" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/DELAY" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/PID" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/TRACE" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/BSP" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/KEY" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/STP" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/JYS" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/SINCOS" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/MAXCI" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/PID_PACK" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/MOTOR" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/STEPPER" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/GANWEI" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/ENCODER" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total" -I"D:/Users/liu jia ming/Desktop/作品/MspM0_Car/Aim_robot/task/total/Debug" -I"D:/Ti/CCS/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"D:/Ti/CCS/mspm0_sdk_2_05_01_00/source" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/ENCODER" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/GANWEI" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/PID_Pack" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/STEPPER" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/MOTOR" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/PID_Pack" -I"C:/Users/Lenovo/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/MAXCI" -gdwarf-3 -MMD -MP -MF"MOTOR/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


