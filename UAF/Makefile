KDIR  = /lib/modules/`uname -r`/build

uaf_driver-y := uaf.o
obj-m 	     := uaf_driver.o

kbuild: 
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean
