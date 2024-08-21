# cloudlinux

Enable debug output. 
`sudo echo "module_debug +p" > sudo /sys/kernel/debug/dynamic_debug/control`

Add module
`sudo insmod hello_kernelcare.ko`

Show message module
`dmesg | tail -n 10`

Delete module
`sudo rmmod hello_kernelcare.ko`
