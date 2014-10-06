all:
	cd lib && make all
	cd app && make all
	cd test && make all

clean:
	cd lib && make clean
	cd app && make clean
	cd test && make clean

distclean:
	cd lib && make distclean
	cd app && make distclean
	cd test && make distclean

install:
	cd lib && make install
	cd app && make install
	cd test && make install

uninstall:
	cd lib && make uninstall
	cd app && make uninstall
	cd test && make uninstall
