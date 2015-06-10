############################################################
#
# msmwctl
#
############################################################
SHELL=/bin/bash

TARGET=usr/bin/msmwctl
VERSION=0.0.6
all : $(TARGET)

cross   =
sysroot =
DESTDIR =
prefix = /usr/local
bindir = $(prefix)/bin
resdir = $(prefix)/share/msmwctl

###################################################################

SUBDIRS = src
SOURCES = $(foreach s,$(SUBDIRS),$(wildcard $(s)/*.c $(s)/*.cc))
HEADERS = $(foreach s,$(SUBDIRS),$(wildcard $(s)/*.h))
MODULES = $(foreach s,$(SOURCES),$(addsuffix .o,$(basename $(s))))

INCLUDES += $(foreach s,$(SUBDIRS),-I$(s))
DEFINES  += -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS


###################################################################

# C preprocessor flags
CPP = $(cross)cpp
CPPFLAGS += $(DEFINES) $(INCLUDES)

# C Compiler and flags
CC = $(cross)gcc -std=c11
CFLAGS += -Wall -Wextra -O3 -g3

# C++ Compiler and flags
CXX = $(cross)g++ -std=c++11
CXXFLAGS += $(CFLAGS)

LD=$(CXX)
LDFLAGS = $(CXXFLAGS) 

STRIP=$(cross)strip --strip-all
# STRIP = echo not stripping  

SYSTEMCTL=$(shell which systemctl)
SERVICE=$(shell which service)

LDLIBS  += -lwthttp -lwt -lmsmctl -ljansson  \
  -lboost_random -lboost_regex \
  -lboost_signals -lboost_system -lboost_thread -lboost_filesystem \
  -lboost_program_options -lboost_date_time -lpthread

###################################################################



PRECOMPILED_HEADER = src/msmwctl.h.gch
$(PRECOMPILED_HEADER) : src/msmwctl.h src/msm.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) src/msmwctl.h -o $@


$(MODULES): $(HEADERS) $(PRECOMPILED_HEADER)
$(TARGET) : $(MODULES) Makefile
	$(LD) $(LDFLAGS) -o $@ $(MODULES) $(LDLIBS) && $(STRIP) $@ 

clean:
	$(RM) $(MODULES)

distclean: clean
	$(RM) $(TARGET) src/*.h.gch

install: $(TARGET) $(DESTDIR)/$(bindir) $(DESTDIR)/$(resdir) 
	cp usr/bin/msmwctl $(DESTDIR)/$(bindir)/msmwctl
	cp -r usr/bin/* $(DESTDIR)/$(resdir) && rm -f $(DESTDIR)/$(resdir)/msmwctl 
	mkdir -p $(DESTDIR)/etc/systemd/system && sed -e "s:BINDIR:$(bindir):g" etc/systemd/system/msmwctl.service > $(DESTDIR)/etc/systemd/system/msmwctl.service
	mkdir -p $(DESTDIR)/etc/init.d && sed -e "s:BINDIR:$(bindir):g" etc/init.d/msmwctl > $(DESTDIR)/etc/init.d/msmwctl && chmod a+x $(DESTDIR)/etc/init.d/msmwctl
	sed -e "s:DOCROOT:$(resdir):g" -e "s:APPROOT:$(resdir):g" etc/msmwctl.cfg > $(DESTDIR)/etc/msmwctl.cfg	
	[ -z $(DESTDIR) ] && [ ! -z $(SYSTEMCTL) ] && $(SYSTEMCTL) daemon-reload || echo "skip daemon-reload"


uninstall:
	if [ -z $(DESTDIR) ] && [ ! -z $(SERVICE) ] ; then $(SERVICE) msmwctl stop ; update-rc.d msmwctl remove ; fi
	if [ -z $(DESTDIR) ] && [ ! -z $(SYSTEMCTL) ] ; then $(SYSTEMCTL) stop msmwctl ; $(SYSTEMCTL) disable msmwctl ; fi
	rm -f $(DESTDIR)/etc/systemd/system/msmwctl.service $(DESTDIR)/etc/init.d/msmwctl
	[ -z $(DESTDIR) ] && [ ! -z $(SYSTEMCTL) ] && $(SYSTEMCTL) daemon-reload || echo "skip daemon-reload"
	rm -f $(DESTDIR)/$(bindir)/msmwctl
	rm -rf $(DESTDIR)/$(resdir)

$(DESTDIR)/$(bindir): ;	mkdir -p $@
$(DESTDIR)/$(resdir): ;	mkdir -p $@
