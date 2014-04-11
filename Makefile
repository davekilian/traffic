all:
	cd lib && make all
	cd app && make all

clean:
	cd lib && make clean
	cd app && make clean

distclean:
	cd lib && make distclean
	cd app && make distclean

install:
	cd lib && make install
	cd app && make install

uninstall:
	cd lib && make uninstall
	cd app && make uninstall
