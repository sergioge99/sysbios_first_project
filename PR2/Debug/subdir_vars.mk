################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../app.cfg 

CMD_SRCS += \
../TM4C123GH6PM.cmd 

LIB_SRCS += \
../driverlib.lib 

C_SRCS += \
../Event.c \
../computos.c \
../comun.c \
../servidores.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./Event.d \
./computos.d \
./comun.d \
./servidores.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./Event.obj \
./computos.obj \
./comun.obj \
./servidores.obj 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"Event.obj" \
"computos.obj" \
"comun.obj" \
"servidores.obj" 

C_DEPS__QUOTED += \
"Event.d" \
"computos.d" \
"comun.d" \
"servidores.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

C_SRCS__QUOTED += \
"../Event.c" \
"../computos.c" \
"../comun.c" \
"../servidores.c" 


