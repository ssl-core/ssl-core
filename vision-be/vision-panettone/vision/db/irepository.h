#ifndef VISION_DB_IREPOSITORY_H
#define VISION_DB_IREPOSITORY_H

#include <future>
#include <optional>
#include <vector>

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
  [[nodiscard]] virtual std::optional<std::vector<data_type>>
  findRange(const id_type& key_lower_bound, const id_type& key_upper_bound) = 0;
};

} // namespace vision

#endif // VISION_DB_IREPOSITORY_H
