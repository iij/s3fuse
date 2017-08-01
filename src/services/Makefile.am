SUBDIRS = .
DIST_SUBDIRS = . aws iijgio gs

if WITH_AWS
SUBDIRS += aws
endif

if WITH_IIJGIO
SUBDIRS += iijgio
endif

if WITH_GS
SUBDIRS += gs
endif

noinst_LTLIBRARIES = libs3fuse_services.la

libs3fuse_services_la_SOURCES = \
	file_transfer.cc \
	file_transfer.h \
	impl.cc \
	impl.h \
	service.cc \
	service.h
