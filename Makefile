include common.mk

define make_wrapper
	$(if $(shell if [ -e /etc/arm_devenv_guest ]; then echo "guest"; fi), \
	  # VirtualBox guest VM
	  $(MAKE) -f $(RULE_FILE) $(1), \
	  # Host
	  $(if $(shell ssh -F .ssh_config default "exit"; \
	  		if [ $$? != 0 ]; then echo "no-guest"; fi), \
	  	vagrant halt
	  	vagrant up)
	  ssh -F .ssh_config default "cd /vagrant/; \
		  env MAKEFLAGS=$(MAKEFLAGS) make -f $(RULE_FILE) $(1)"
	)
endef

.PHONY: all qemu arm-sync help

all:
	@echo nop

qemu:
	$(call make_wrapper,qemu)

arm-sync: # tentative
	$(call make_wrapper,arm-sync)

help:
	@grep '^[^.#[:space:]].*:' Makefile
