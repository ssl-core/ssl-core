#ifndef VISION_DB_IMONGODB_REPOSITORY_H
#define VISION_DB_IMONGODB_REPOSITORY_H

#include "vision/db/irepository.h"

#include <future>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

namespace vision {

struct MongoDbRepositoryBuildArgs {
  std::string uri;
  std::string db_name;
  std::string collection_name;
};

template <class Id, class Data>
class IMongoDbRepository : public IRepository<Id, Data> {
 public:
  virtual ~IMongoDbRepository() = default;

  explicit IMongoDbRepository(const MongoDbRepositoryBuildArgs& args) :
      client_{mongocxx::uri{args.uri}},
      db_{client_[args.db_name]},
      collection_{db_[args.collection_name]} {}

  std::future<bool> connect() final {
    using namespace std::chrono_literals;

    static constexpr auto kSleepTime = 5s;
    static constexpr int kMaxAttemps = 12; // 1min total.

    return std::async(std::launch::async, [this]() -> bool {
      using bsoncxx::builder::basic::make_document;
      using bsoncxx::builder::basic::kvp;

      const auto kPing = make_document(kvp("ping", 1));

      for (int attempt = 0; attempt < kMaxAttemps; ++attempt) {
        int64_t rounded_elapsed_time = kSleepTime.count() * (1 + attempt);

        try {
          db_.drop();
          db_.run_command(kPing.view());

          std::cout << "connected to MongoDB after " << rounded_elapsed_time << "s.\n";

          return true;
        } catch (const std::exception& e) {
          std::cerr << "error while connecting to MongoDB: " << e.what() << ".\n";
          std::cerr << "retrying in " << kSleepTime.count()
                    << " seconds (total: " << rounded_elapsed_time << "s)...\n";

          std::this_thread::sleep_for(kSleepTime);
        }
      }

      return false;
    });
  }

 protected:
  // NOLINTBEGIN(*non-private*)
  mongocxx::instance instance_;
  mongocxx::client client_;
  mongocxx::database db_;
  mongocxx::collection collection_;
  // NOLINTEND(*non-private*)
};

} // namespace vision

#endif // VISION_DB_IMONGODB_REPOSITORY_H
