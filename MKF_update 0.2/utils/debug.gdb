target remote localhost:3333
monitor reset halt
monitor stm32f1x unlock 0
monitor stm32f1x mass_erase 0
set remotetimeout 5
load
monitor reset halt
break main
break assert_failed
commands
print file
print line
end
#break rtos_assert_failed
#commands
#print file
#print line
#end
#break vApplicationStackOverflowHook
#commands
#print pcTaskName
#end
#break vApplicationMallocFailedHook
#continue
