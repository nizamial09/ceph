// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef RBD_MIRROR_IMAGE_SYNC_THROTTLER_H
#define RBD_MIRROR_IMAGE_SYNC_THROTTLER_H

#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include "common/Mutex.h"
#include "common/config_obs.h"

class CephContext;
class Context;

namespace ceph { class Formatter; }
namespace librbd { class ImageCtx; }

namespace rbd {
namespace mirror {

template <typename ImageCtxT = librbd::ImageCtx>
class ImageSyncThrottler : public md_config_obs_t {
public:
  static ImageSyncThrottler *create(CephContext *cct) {
    return new ImageSyncThrottler(cct);
  }
  void destroy() {
    delete this;
  }

  ImageSyncThrottler(CephContext *cct);
  ~ImageSyncThrottler() override;

  void set_max_concurrent_syncs(uint32_t max);
  void start_op(const std::string &id, Context *on_start);
  bool cancel_op(const std::string &id);
  void finish_op(const std::string &id);
  void drain(int r);

  void print_status(Formatter *f, std::stringstream *ss);

private:
  CephContext *m_cct;
  Mutex m_lock;
  uint32_t m_max_concurrent_syncs;
  std::list<std::string> m_queue;
  std::map<std::string, Context *> m_queued_ops;
  std::set<std::string> m_inflight_ops;

  const char **get_tracked_conf_keys() const override;
  void handle_conf_change(const ConfigProxy& conf,
                          const std::set<std::string> &changed) override;
};

} // namespace mirror
} // namespace rbd

extern template class rbd::mirror::ImageSyncThrottler<librbd::ImageCtx>;

#endif // RBD_MIRROR_IMAGE_SYNC_THROTTLER_H
