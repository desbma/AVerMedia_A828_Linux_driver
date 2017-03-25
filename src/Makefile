ifneq ($(KERNELDIR),)
KERNELSRC=$(KERNELDIR)
KERNELOBJ=$(KERNELDIR)
else
        KERNVER = $(shell uname -r)
        ifeq ($(KERNELSRC),)
        KERNELSRC= /lib/modules/$(KERNVER)/source
        endif

        ifeq ($(KERNELOBJ),)
        KERNELOBJ= /lib/modules/$(KERNVER)/build
        endif
endif


COBJS = a828-core.o aver/osdep_dvb.o 	aver/osdep_th2.o aver/osdep_v4l2.o aver/osdep_vbuf.o 	aver/osdep_alsa.o


PREBUILD_OBJ = _prebuild.o


extra-version := $(shell egrep -e '^EXTRAVERSION' $(KERNELOBJ)/Makefile | awk 'BEGIN{FS="="}{print $$2}' | tr -d ' ')


ifeq ($(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(extra-version),2.6.15-1.2054_FC5)
        EXTRA_CFLAGS += -DUSB_FC5_HACK=1
endif
ifeq ($(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(extra-version),2.6.15-1.2054_FC5smp)
        EXTRA_CFLAGS += -DUSB_FC5_HACK=1
endif


ifeq ($(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(extra-version),2.6.12-rc4)
        EXTRA_CFLAGS += -DDVB_2612_HACK=1
endif


ifeq ($(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(extra-version),2.6.11-1.1369_FC4)
        EXTRA_CFLAGS += -DDVB_FC4_HACK=1
endif

ifeq ($(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(extra-version),2.6.11-1.1369_FC4smp)
        EXTRA_CFLAGS += -DDVB_FC4_HACK=1
endif


EXTRA_CFLAGS += -I$(KERNELSRC)/drivers/media/dvb/dvb-core -DUSE_SWIC2 		-D__AVER_USB_PRE__=AVerA828_ -D__AVER_USB_PRE_NAME__="\"A828\""





MODEL := $(shell cat $(SUBDIRS)/.model 2>/dev/null)
obj-m := a828.o averusba828.o
a828-objs := $(COBJS) $(PREBUILD_OBJ)
averusba828-objs := aver/averusb-mod.o
default:: links
	$(MAKE) -C $(KERNELSRC) O=$(KERNELOBJ) SUBDIRS=`pwd` $(TARGET)
	$(CROSS_COMPILE)strip --strip-debug *.ko




clean::
	-rm -rf *~ *.o *.ko *.mod.c .tmp_versions aver/*.o aver/*.o.cmd
	-rm -f .version .exversion .*.o.flags .*.o.d .*.o.cmd .*.ko.cmd
	-rm -f Module.symvers Modules.symvers



links:: $(LINKS)
