TEMPLATE      = subdirs
SUBDIRS       = \
	      src/ms_wizard/sources/test \
              src/ms_struct \
              src/ms_wizard
DESTDIR=bin
DESTDIR_TARGET=bin
DISTFILES=test master_struct fsm_wizard
CONFIG+=build_all
message("The project contains the following files:")
