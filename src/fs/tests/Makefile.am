noinst_PROGRAMS = \
	callback_xattr \
	decrypt_file \
	encrypt_file \
	get_mime_type \
	static_xattr

callback_xattr_SOURCES = callback_xattr.cc
callback_xattr_LDADD = ../libs3fuse_fs.la ../../base/libs3fuse_base.la ../../crypto/libs3fuse_crypto.la $(LDADD)

decrypt_file_SOURCES = decrypt_file.cc
decrypt_file_LDADD = ../../crypto/libs3fuse_crypto.la $(LDADD)

encrypt_file_SOURCES = encrypt_file.cc
encrypt_file_LDADD = ../../crypto/libs3fuse_crypto.la $(LDADD)

get_mime_type_SOURCES = get_mime_type.cc
get_mime_type_LDADD = ../libs3fuse_fs.la ../../base/libs3fuse_base.la $(LDADD)

static_xattr_SOURCES = static_xattr.cc
static_xattr_LDADD = ../libs3fuse_fs.la ../../crypto/libs3fuse_crypto.la $(LDADD)
