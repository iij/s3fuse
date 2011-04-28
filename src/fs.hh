#ifndef S3_FS_HH
#define S3_FS_HH

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <time.h>

#include <map>
#include <string>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <pugixml/pugixml.hpp>

#include "object_cache.hh"
#include "request.hh"
#include "thread_pool.hh"
#include "work_item.hh"

#define ASYNC_CALL_FG(fn, ...) \
  do { \
    work_item::ptr wi(new work_item(boost::bind(&fs::__ ## fn, this, _1, ## __VA_ARGS__))); \
    \
    _fg_thread_pool.post(wi); \
    return wi->wait(); \
  } while (0)

#define ASYNC_CALL_NONBLOCK_BG(fn, ...) \
  do { \
    work_item::ptr wi(new work_item(boost::bind(&fs::__ ## fn, this, _1, ## __VA_ARGS__))); \
    \
    _bg_thread_pool.post(wi); \
  } while (0)

#define ASYNC_DECL(fn, ...) int __ ## fn (const request::ptr &req, ## __VA_ARGS__);
#define ASYNC_DEF(fn, ...) int fs::__ ## fn (const request::ptr &req, ## __VA_ARGS__)

namespace s3
{
  class fs
  {
  public:
    fs();
    ~fs();

    // the cast to std::string * is required to prevent boost from trying to pass a long int
    inline int get_stats(const std::string &path, struct stat *s) { ASYNC_CALL_FG(get_stats, path, s, HINT_NONE); }
    inline int read_directory(const std::string &path, fuse_fill_dir_t filler, void *buf) { ASYNC_CALL_FG(read_directory, path, filler, buf); }
    inline int create_object(const std::string &path, mode_t mode) { ASYNC_CALL_FG(create_object, path, mode); }
    inline int change_metadata(const std::string &path, mode_t mode, uid_t uid, gid_t gid) { ASYNC_CALL_FG(change_metadata, path, mode, uid, gid); }
    inline int open(const std::string &path, uint64_t *context) { ASYNC_CALL_FG(open, path, context); }
    inline int flush(uint64_t context) { ASYNC_CALL_FG(flush, context); }
    inline int close(uint64_t context) { ASYNC_CALL_FG(close, context); }
    inline int remove_file(const std::string &path) { ASYNC_CALL_FG(remove_object, path); }
    inline int remove_directory(const std::string &path) { ASYNC_CALL_FG(remove_object, path); }
    inline int rename_object(const std::string &from, const std::string &to) { ASYNC_CALL_FG(rename_object, from, to); }

    int read(char *buffer, size_t size, off_t offset, uint64_t context);
    int write(const char *buffer, size_t size, off_t offset, uint64_t context);
    int release(uint64_t context);

  private:
    enum hint
    {
      HINT_NONE    = 0x0,
      HINT_IS_DIR  = 0x1,
      HINT_IS_FILE = 0x2
    };

    enum file_status
    {
      FS_NONE     = 0x0,
      FS_DIRTY    = 0x1,
      FS_FLUSHING = 0x2,
      FS_IN_USE   = 0x4
    };

    typedef std::map<std::string, std::string> string_map;

    struct file_handle
    {
      int status;
      std::string path;
      std::string etag;
      std::string content_type;
      string_map metadata;
      FILE *local_fd;
    };

    typedef boost::shared_ptr<file_handle> handle_ptr;
    typedef std::map<uint64_t, handle_ptr> handle_map;

    ASYNC_DECL(get_stats,       const std::string &path, struct stat *s, int hints);
    ASYNC_DECL(prefill_stats,   const std::string &path, int hints);
    ASYNC_DECL(read_directory,  const std::string &path, fuse_fill_dir_t filler, void *buf);
    ASYNC_DECL(create_object,   const std::string &path, mode_t mode);
    ASYNC_DECL(change_metadata, const std::string &path, mode_t mode, uid_t uid, gid_t gid);
    ASYNC_DECL(open,            const std::string &path, uint64_t *context);
    ASYNC_DECL(flush,           uint64_t context);
    ASYNC_DECL(close,           uint64_t context);
    ASYNC_DECL(remove_object,   const std::string &path);
    ASYNC_DECL(rename_object,   const std::string &from, const std::string &to);

    int flush(const request::ptr &req, const handle_ptr &handle);

    object::ptr get_object(const request::ptr &req, const std::string &path, int hints = HINT_NONE);
    int remove_object(const request::ptr &req, const object::ptr &obj);

    pugi::xpath_query _prefix_query;
    pugi::xpath_query _key_query;
    thread_pool _fg_thread_pool, _bg_thread_pool;
    object_cache _object_cache;
    handle_map _open_files;
    boost::mutex _open_files_mutex;
    uint64_t _next_open_file_handle;
  };
}

#endif
