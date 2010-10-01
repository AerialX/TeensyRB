TARGET		:= controller
SOURCE		:= main.cpp
INCLUDE		:= libwiiuse/src
LIBS		:= parapin

LIBWIIUSE	:= libwiiuse/libwiiuse.so

CFLAGS		:= -O2 -Wall $(foreach inc,$(INCLUDE),-I$(inc))
CXXFLAGS	:= $(CFLAGS)
LDFLAGS		:= $(foreach lib,$(LIBS),-l$(lib)) $(LIBWIIUSE)
CXX			:= g++

all: $(TARGET)

clean:
	rm -f $(TARGET)
	make -C libwiiuse clean

$(LIBWIIUSE):
	make -C libwiiuse

$(TARGET): $(SOURCE) $(LIBWIIUSE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $@ $(LDFLAGS)
