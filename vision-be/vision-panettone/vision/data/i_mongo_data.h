// IMongoData.h
#ifndef I_MONGO_DATA_H
#define I_MONGO_DATA_H

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>

class IMongoData {
 public:
  IMongoData() = default;
  virtual ~IMongoData() = default;

  [[nodiscard]] virtual bsoncxx::builder::stream::document getMongoDocument() const = 0;
};

#endif // I_MONGO_DATA_H
