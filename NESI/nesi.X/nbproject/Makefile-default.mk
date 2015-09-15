#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/nesi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/nesi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/drivers/button.c ../src/drivers/camera.c ../src/drivers/co2.c ../src/drivers/datalog.c ../src/drivers/file.c ../src/drivers/led.c ../src/drivers/moisture.c ../src/drivers/powerDriver.c ../src/drivers/sdcard.c ../src/drivers/system.c ../src/drivers/usb.c ../src/mdd/FSIO.c ../src/mdd/SD-SPI.c ../src/usb/usb_descriptors.c ../src/usb/usb_device.c ../src/usb/usb_function_msd.c ../src/main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1639450193/button.o ${OBJECTDIR}/_ext/1639450193/camera.o ${OBJECTDIR}/_ext/1639450193/co2.o ${OBJECTDIR}/_ext/1639450193/datalog.o ${OBJECTDIR}/_ext/1639450193/file.o ${OBJECTDIR}/_ext/1639450193/led.o ${OBJECTDIR}/_ext/1639450193/moisture.o ${OBJECTDIR}/_ext/1639450193/powerDriver.o ${OBJECTDIR}/_ext/1639450193/sdcard.o ${OBJECTDIR}/_ext/1639450193/system.o ${OBJECTDIR}/_ext/1639450193/usb.o ${OBJECTDIR}/_ext/659861939/FSIO.o ${OBJECTDIR}/_ext/659861939/SD-SPI.o ${OBJECTDIR}/_ext/659870090/usb_descriptors.o ${OBJECTDIR}/_ext/659870090/usb_device.o ${OBJECTDIR}/_ext/659870090/usb_function_msd.o ${OBJECTDIR}/_ext/1360937237/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1639450193/button.o.d ${OBJECTDIR}/_ext/1639450193/camera.o.d ${OBJECTDIR}/_ext/1639450193/co2.o.d ${OBJECTDIR}/_ext/1639450193/datalog.o.d ${OBJECTDIR}/_ext/1639450193/file.o.d ${OBJECTDIR}/_ext/1639450193/led.o.d ${OBJECTDIR}/_ext/1639450193/moisture.o.d ${OBJECTDIR}/_ext/1639450193/powerDriver.o.d ${OBJECTDIR}/_ext/1639450193/sdcard.o.d ${OBJECTDIR}/_ext/1639450193/system.o.d ${OBJECTDIR}/_ext/1639450193/usb.o.d ${OBJECTDIR}/_ext/659861939/FSIO.o.d ${OBJECTDIR}/_ext/659861939/SD-SPI.o.d ${OBJECTDIR}/_ext/659870090/usb_descriptors.o.d ${OBJECTDIR}/_ext/659870090/usb_device.o.d ${OBJECTDIR}/_ext/659870090/usb_function_msd.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1639450193/button.o ${OBJECTDIR}/_ext/1639450193/camera.o ${OBJECTDIR}/_ext/1639450193/co2.o ${OBJECTDIR}/_ext/1639450193/datalog.o ${OBJECTDIR}/_ext/1639450193/file.o ${OBJECTDIR}/_ext/1639450193/led.o ${OBJECTDIR}/_ext/1639450193/moisture.o ${OBJECTDIR}/_ext/1639450193/powerDriver.o ${OBJECTDIR}/_ext/1639450193/sdcard.o ${OBJECTDIR}/_ext/1639450193/system.o ${OBJECTDIR}/_ext/1639450193/usb.o ${OBJECTDIR}/_ext/659861939/FSIO.o ${OBJECTDIR}/_ext/659861939/SD-SPI.o ${OBJECTDIR}/_ext/659870090/usb_descriptors.o ${OBJECTDIR}/_ext/659870090/usb_device.o ${OBJECTDIR}/_ext/659870090/usb_function_msd.o ${OBJECTDIR}/_ext/1360937237/main.o

# Source Files
SOURCEFILES=../src/drivers/button.c ../src/drivers/camera.c ../src/drivers/co2.c ../src/drivers/datalog.c ../src/drivers/file.c ../src/drivers/led.c ../src/drivers/moisture.c ../src/drivers/powerDriver.c ../src/drivers/sdcard.c ../src/drivers/system.c ../src/drivers/usb.c ../src/mdd/FSIO.c ../src/mdd/SD-SPI.c ../src/usb/usb_descriptors.c ../src/usb/usb_device.c ../src/usb/usb_function_msd.c ../src/main.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/nesi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256GB106
MP_LINKER_FILE_OPTION=,--script=../src/dev/p24FJ256GB106.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1639450193/button.o: ../src/drivers/button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/button.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/button.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/button.c  -o ${OBJECTDIR}/_ext/1639450193/button.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/button.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/button.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/camera.o: ../src/drivers/camera.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/camera.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/camera.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/camera.c  -o ${OBJECTDIR}/_ext/1639450193/camera.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/camera.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/camera.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/co2.o: ../src/drivers/co2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/co2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/co2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/co2.c  -o ${OBJECTDIR}/_ext/1639450193/co2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/co2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/co2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/datalog.o: ../src/drivers/datalog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/datalog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/datalog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/datalog.c  -o ${OBJECTDIR}/_ext/1639450193/datalog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/datalog.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/datalog.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/file.o: ../src/drivers/file.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/file.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/file.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/file.c  -o ${OBJECTDIR}/_ext/1639450193/file.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/file.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/file.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/led.o: ../src/drivers/led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/led.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/led.c  -o ${OBJECTDIR}/_ext/1639450193/led.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/led.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/led.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/moisture.o: ../src/drivers/moisture.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/moisture.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/moisture.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/moisture.c  -o ${OBJECTDIR}/_ext/1639450193/moisture.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/moisture.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/moisture.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/powerDriver.o: ../src/drivers/powerDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/powerDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/powerDriver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/powerDriver.c  -o ${OBJECTDIR}/_ext/1639450193/powerDriver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/powerDriver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/powerDriver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/sdcard.o: ../src/drivers/sdcard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/sdcard.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/sdcard.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/sdcard.c  -o ${OBJECTDIR}/_ext/1639450193/sdcard.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/sdcard.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/sdcard.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/system.o: ../src/drivers/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/system.c  -o ${OBJECTDIR}/_ext/1639450193/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/usb.o: ../src/drivers/usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/usb.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/usb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/usb.c  -o ${OBJECTDIR}/_ext/1639450193/usb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/usb.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/usb.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/659861939/FSIO.o: ../src/mdd/FSIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659861939" 
	@${RM} ${OBJECTDIR}/_ext/659861939/FSIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/659861939/FSIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/mdd/FSIO.c  -o ${OBJECTDIR}/_ext/659861939/FSIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/659861939/FSIO.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/659861939/FSIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/659861939/SD-SPI.o: ../src/mdd/SD-SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659861939" 
	@${RM} ${OBJECTDIR}/_ext/659861939/SD-SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/659861939/SD-SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/mdd/SD-SPI.c  -o ${OBJECTDIR}/_ext/659861939/SD-SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/659861939/SD-SPI.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/659861939/SD-SPI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/659870090/usb_descriptors.o: ../src/usb/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659870090" 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/usb/usb_descriptors.c  -o ${OBJECTDIR}/_ext/659870090/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/659870090/usb_descriptors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/659870090/usb_descriptors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/659870090/usb_device.o: ../src/usb/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659870090" 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/usb/usb_device.c  -o ${OBJECTDIR}/_ext/659870090/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/659870090/usb_device.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/659870090/usb_device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/659870090/usb_function_msd.o: ../src/usb/usb_function_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659870090" 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_function_msd.o.d 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_function_msd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/usb/usb_function_msd.c  -o ${OBJECTDIR}/_ext/659870090/usb_function_msd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/659870090/usb_function_msd.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/659870090/usb_function_msd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/main.c  -o ${OBJECTDIR}/_ext/1360937237/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1639450193/button.o: ../src/drivers/button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/button.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/button.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/button.c  -o ${OBJECTDIR}/_ext/1639450193/button.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/button.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/button.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/camera.o: ../src/drivers/camera.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/camera.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/camera.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/camera.c  -o ${OBJECTDIR}/_ext/1639450193/camera.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/camera.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/camera.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/co2.o: ../src/drivers/co2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/co2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/co2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/co2.c  -o ${OBJECTDIR}/_ext/1639450193/co2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/co2.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/co2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/datalog.o: ../src/drivers/datalog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/datalog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/datalog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/datalog.c  -o ${OBJECTDIR}/_ext/1639450193/datalog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/datalog.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/datalog.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/file.o: ../src/drivers/file.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/file.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/file.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/file.c  -o ${OBJECTDIR}/_ext/1639450193/file.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/file.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/file.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/led.o: ../src/drivers/led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/led.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/led.c  -o ${OBJECTDIR}/_ext/1639450193/led.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/led.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/led.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/moisture.o: ../src/drivers/moisture.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/moisture.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/moisture.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/moisture.c  -o ${OBJECTDIR}/_ext/1639450193/moisture.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/moisture.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/moisture.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/powerDriver.o: ../src/drivers/powerDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/powerDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/powerDriver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/powerDriver.c  -o ${OBJECTDIR}/_ext/1639450193/powerDriver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/powerDriver.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/powerDriver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/sdcard.o: ../src/drivers/sdcard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/sdcard.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/sdcard.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/sdcard.c  -o ${OBJECTDIR}/_ext/1639450193/sdcard.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/sdcard.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/sdcard.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/system.o: ../src/drivers/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/system.c  -o ${OBJECTDIR}/_ext/1639450193/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/system.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/usb.o: ../src/drivers/usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1639450193" 
	@${RM} ${OBJECTDIR}/_ext/1639450193/usb.o.d 
	@${RM} ${OBJECTDIR}/_ext/1639450193/usb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/usb.c  -o ${OBJECTDIR}/_ext/1639450193/usb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/usb.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/usb.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/659861939/FSIO.o: ../src/mdd/FSIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659861939" 
	@${RM} ${OBJECTDIR}/_ext/659861939/FSIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/659861939/FSIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/mdd/FSIO.c  -o ${OBJECTDIR}/_ext/659861939/FSIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/659861939/FSIO.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/659861939/FSIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/659861939/SD-SPI.o: ../src/mdd/SD-SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659861939" 
	@${RM} ${OBJECTDIR}/_ext/659861939/SD-SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/659861939/SD-SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/mdd/SD-SPI.c  -o ${OBJECTDIR}/_ext/659861939/SD-SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/659861939/SD-SPI.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/659861939/SD-SPI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/659870090/usb_descriptors.o: ../src/usb/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659870090" 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/usb/usb_descriptors.c  -o ${OBJECTDIR}/_ext/659870090/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/659870090/usb_descriptors.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/659870090/usb_descriptors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/659870090/usb_device.o: ../src/usb/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659870090" 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/usb/usb_device.c  -o ${OBJECTDIR}/_ext/659870090/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/659870090/usb_device.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/659870090/usb_device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/659870090/usb_function_msd.o: ../src/usb/usb_function_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659870090" 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_function_msd.o.d 
	@${RM} ${OBJECTDIR}/_ext/659870090/usb_function_msd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/usb/usb_function_msd.c  -o ${OBJECTDIR}/_ext/659870090/usb_function_msd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/659870090/usb_function_msd.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/659870090/usb_function_msd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/main.c  -o ${OBJECTDIR}/_ext/1360937237/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d"        -g -omf=elf -mlarge-data -msmall-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/nesi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../src/dev/p24FJ256GB106.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/nesi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf  -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/nesi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../src/dev/p24FJ256GB106.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/nesi.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/nesi.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
