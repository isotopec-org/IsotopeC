
Debian
====================
This directory contains files used to package isotopecd/isotopec-qt
for Debian-based Linux systems. If you compile isotopecd/isotopec-qt yourself, there are some useful files here.

## isotopec: URI support ##


isotopec-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install isotopec-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your isotopec-qt binary to `/usr/bin`
and the `../../share/pixmaps/bitcoin128.png` to `/usr/share/pixmaps`

isotopec-qt.protocol (KDE)
