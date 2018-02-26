.PHONY:clean

obj-m := gpio_read.o

KERNEL_DIR=/home/share/linux-3.4.2

all:
	$(MAKE) -C $(KERNEL_DIR) M=`pwd` modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=`pwd` modules clean
	rm -rf modules.order



