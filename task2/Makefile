obj-m += jiffies_module.o

CFLAGS_jiffies_module.o := -DDEBUG

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean