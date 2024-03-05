#ifndef VISION_DB_IREPOSITORY_H
#define VISION_DB_IREPOSITORY_H

#include <future>
#include <optional>

namespace vision {

template <class Id, class Data>
class IRepository {
 public:
  using id_type = Id;
  using data_type = Data;

  virtual ~IRepository() = default;

  virtual std::future<bool> connect() = 0;

  virtual void save(const data_type& data) = 0;
  virtual void remove(const id_type& key) = 0;

  [[nodiscard]] virtual std::optional<data_type> find(const id_type& key) = 0;
  // TODO($ISSUE_N): Add find by range.
};

} // namespace vision

#endif // VISION_DB_IREPOSITORY_H
