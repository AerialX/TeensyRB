#
#	wiiuse Makefile
#

all clean install:
	@$(MAKE) -C src $@

wiiuse:
	@$(MAKE) -C src

distclean:
	@$(MAKE) -C src $@
	@find . -type f \( -name "*.save" -or -name "*~" -or -name gmon.out \) -delete &> /dev/null
