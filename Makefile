all:
	cd 1tox ; $(MAKE)
	cd cc ; $(MAKE)

clean:
	cd 1tox ; $(MAKE) clean
	cd cc ; $(MAKE) clean
