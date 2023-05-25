define FIND_SRC_FILE
	$(addprefix $(1)/,$(notdir $(patsubst %.c,%.o,$(wildcard $(dir $(abspath $(lastword $(MAKEFILE_LIST))))$(1)/*.c))))
endef

ifeq ($(KERNELRELEASE), )
KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD :=$(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR)  M=$(PWD)
clean:
	rm -rf .tmp_versions Module.symvers *.mod *.mod.c *.o *.ko .*.cmd built-in.a Module.markers modules.order .cache.mk
load:
	insmod uio.ko
unload:
	rmmod uio
install:
	cp uio.ko /lib/modules/$(shell uname -r)/kernel/drivers/uio.ko
else
	obj-m := uio.o
	uio-objs := \
		$(call FIND_SRC_FILE,.)
endif
