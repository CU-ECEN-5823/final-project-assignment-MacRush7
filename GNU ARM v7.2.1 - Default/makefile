################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/main-src/subdir.mk
-include src/subdir.mk
-include protocol/bluetooth/lib/EFR32XG13X/GCC/subdir.mk
-include protocol/bluetooth/bt_mesh/src/subdir.mk
-include protocol/bluetooth/ble_stack/src/soc/subdir.mk
-include platform/service/sleeptimer/src/subdir.mk
-include platform/radio/rail_lib/plugin/coexistence/hal/efr32/subdir.mk
-include platform/radio/rail_lib/plugin/coexistence/common/subdir.mk
-include platform/middleware/glib/glib/subdir.mk
-include platform/middleware/glib/dmd/display/subdir.mk
-include platform/emlib/src/subdir.mk
-include platform/emdrv/sleep/src/subdir.mk
-include platform/emdrv/nvm3/src/subdir.mk
-include platform/emdrv/gpiointerrupt/src/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/subdir.mk
-include hardware/kit/common/drivers/subdir.mk
-include hardware/kit/common/bsp/subdir.mk
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
all: project-level-new.axf

# Tool invocations
project-level-new.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:\Users\rualjim\SimplicityStudio\v4_workspace\project-level-new\efr32bg13p632f512gm48.ld" -Wl,--undefined,sl_app_properties,--undefined,__Vectors,--undefined,__aeabi_uldivmod,--undefined,ceil,--undefined,__nvm3Base -Xlinker -no-enum-size-warning -Xlinker -no-wchar-size-warning -Xlinker --gc-sections -Xlinker -Map="project-level-new.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -u _printf_float -o project-level-new.axf -Wl,--start-group "./platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.o" "./src/main-src/cmu.o" "./src/main-src/display.o" "./src/main-src/em_letimer.o" "./src/main-src/gecko_ble_errors.o" "./src/main-src/gpio.o" "./src/main-src/i2c.o" "./src/main-src/letimer.o" "./src/main-src/log.o" "./src/main-src/mcp9808.o" "./src/main-src/push_button.o" "./src/main-src/state.o" "./src/main.o" "./protocol/bluetooth/bt_mesh/src/bg_application_properties.o" "./protocol/bluetooth/bt_mesh/src/mesh_lib.o" "./protocol/bluetooth/bt_mesh/src/mesh_sensor.o" "./protocol/bluetooth/bt_mesh/src/mesh_serdeser.o" "./protocol/bluetooth/ble_stack/src/soc/coexistence-ble.o" "./protocol/bluetooth/ble_stack/src/soc/coexistence_counters-ble.o" "./platform/service/sleeptimer/src/sl_sleeptimer.o" "./platform/service/sleeptimer/src/sl_sleeptimer_hal_rtcc.o" "./platform/radio/rail_lib/plugin/coexistence/hal/efr32/coexistence-hal.o" "./platform/radio/rail_lib/plugin/coexistence/common/coexistence.o" "./platform/middleware/glib/glib/bmp.o" "./platform/middleware/glib/glib/glib.o" "./platform/middleware/glib/glib/glib_bitmap.o" "./platform/middleware/glib/glib/glib_circle.o" "./platform/middleware/glib/glib/glib_font_narrow_6x8.o" "./platform/middleware/glib/glib/glib_font_normal_8x8.o" "./platform/middleware/glib/glib/glib_font_number_16x20.o" "./platform/middleware/glib/glib/glib_line.o" "./platform/middleware/glib/glib/glib_polygon.o" "./platform/middleware/glib/glib/glib_rectangle.o" "./platform/middleware/glib/glib/glib_string.o" "./platform/middleware/glib/dmd/display/dmd_display.o" "./platform/emlib/src/em_assert.o" "./platform/emlib/src/em_burtc.o" "./platform/emlib/src/em_cmu.o" "./platform/emlib/src/em_core.o" "./platform/emlib/src/em_cryotimer.o" "./platform/emlib/src/em_crypto.o" "./platform/emlib/src/em_emu.o" "./platform/emlib/src/em_eusart.o" "./platform/emlib/src/em_gpio.o" "./platform/emlib/src/em_i2c.o" "./platform/emlib/src/em_msc.o" "./platform/emlib/src/em_rmu.o" "./platform/emlib/src/em_rtcc.o" "./platform/emlib/src/em_se.o" "./platform/emlib/src/em_system.o" "./platform/emlib/src/em_timer.o" "./platform/emlib/src/em_usart.o" "./platform/emdrv/sleep/src/sleep.o" "./platform/emdrv/nvm3/src/nvm3_default.o" "./platform/emdrv/nvm3/src/nvm3_hal_flash.o" "./platform/emdrv/nvm3/src/nvm3_lock.o" "./platform/emdrv/gpiointerrupt/src/gpiointerrupt.o" "./platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.o" "./hardware/kit/common/drivers/display.o" "./hardware/kit/common/drivers/displayls013b7dh03.o" "./hardware/kit/common/drivers/displaypalemlib.o" "./hardware/kit/common/drivers/i2cspm.o" "./hardware/kit/common/drivers/mx25flash_spi.o" "./hardware/kit/common/drivers/retargetio.o" "./hardware/kit/common/drivers/retargetserial.o" "./hardware/kit/common/drivers/udelay.o" "./hardware/kit/common/bsp/bsp_stk.o" "./app.o" "./dcd.o" "./gatt_db.o" "./graphics.o" "./init_app.o" "./init_board.o" "./init_mcu.o" "./lcd_driver.o" "./pti.o" "C:\Users\rualjim\SimplicityStudio\v4_workspace\project-level-new\platform\radio\rail_lib\autogen\librail_release\librail_efr32xg13_gcc_release.a" "C:\Users\rualjim\SimplicityStudio\v4_workspace\project-level-new\platform\emdrv\nvm3\lib\libnvm3_CM4_gcc.a" "C:\Users\rualjim\SimplicityStudio\v4_workspace\project-level-new\protocol\bluetooth\lib\EFR32XG13X\GCC\libbluetooth_mesh.a" "C:\Users\rualjim\SimplicityStudio\v4_workspace\project-level-new\protocol\bluetooth\lib\EFR32XG13X\GCC\binapploader.o" -lm "./app_config.o" "./app_src.o" "./src/main-src/gecko_mesh.o" "./src/main-src/main_app.o" -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: project-level-new.hex'
	arm-none-eabi-objcopy -O ihex "project-level-new.axf" "project-level-new.hex"
	@echo ' '

	@echo 'Building bin file: project-level-new.bin'
	arm-none-eabi-objcopy -O binary "project-level-new.axf" "project-level-new.bin"
	@echo ' '

	@echo 'Building s37 file: project-level-new.s37'
	arm-none-eabi-objcopy -O srec "project-level-new.axf" "project-level-new.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "project-level-new.axf" -A
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) project-level-new.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
