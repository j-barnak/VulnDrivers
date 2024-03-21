KDIR  = /lib/modules/`uname -r`/build
obj-m = uaf_driver.o

kbuild: 
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean
