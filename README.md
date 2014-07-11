sasek
=====

A cute little (unfinished, not-really-working) chess game based in Qt UI widgets. It should compile against both Qt4 and Qt5.

So far it's only local multiplayer (the board rotates after each move) and it doesn't check moves. It's basically useless at this point :-) But at least it's cute.

To compile it, run:

	#git clone [cloneurl]
	#cd sasek
	qmake # or qmake-qt4 or qmake-qt5
	make -j4

Then, to run it:
	./sasek
