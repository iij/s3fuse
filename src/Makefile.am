SUBDIRS = base crypto fs services threads .
DIST_SUBDIRS = $(SUBDIRS) tests

if BUILD_TESTS
SUBDIRS += base/tests tests
endif

service_libs =

if WITH_AWS
service_libs += services/aws/libs3fuse_services_aws.la
endif

if WITH_IIJGIO
service_libs += services/iijgio/libs3fuse_services_iijgio.la
endif

if WITH_GS
service_libs += services/gs/libs3fuse_services_gs.la
endif

# the order of archives in s3fuse_libs matters -- they must be listed such that if A
# requires symbols provided by B, then A must appear before B

s3fuse_libs = \
	fs/libs3fuse_fs.la \
	$(service_libs) \
	threads/libs3fuse_threads.la \
	services/libs3fuse_services.la \
	crypto/libs3fuse_crypto.la \
	base/libs3fuse_base.la

init_src = \
	init.cc \
	init.h

bin_PROGRAMS = s3fuse s3fuse_sha256_sum s3fuse_vol_key

if WITH_GS
bin_PROGRAMS += s3fuse_gs_get_token
endif

s3fuse_SOURCES = $(init_src) main.cc operations.cc operations.h
s3fuse_LDADD = $(s3fuse_libs) $(LDADD)

s3fuse_gs_get_token_SOURCES = gs_get_token.cc
s3fuse_gs_get_token_LDADD = $(s3fuse_libs) $(LDADD)

s3fuse_sha256_sum_SOURCES = sha256_sum.cc
s3fuse_sha256_sum_LDADD = $(s3fuse_libs) $(LDADD)

s3fuse_vol_key_SOURCES = $(init_src) vol_key.cc
s3fuse_vol_key_LDADD = $(s3fuse_libs) $(LDADD)
