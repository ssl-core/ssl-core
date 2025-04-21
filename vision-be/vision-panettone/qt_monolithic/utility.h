#ifndef VISION_QT_MONOLITHIC_UTILITY_H
#define VISION_QT_MONOLITHIC_UTILITY_H

#include <mutex>
#include <QObject>
#include <QtCore>
#include <robocin/network/zmq_subscriber_socket.h>
#include <thread>
#include <vector>

namespace vision::qt_monolithic {

inline int64_t GetDateTimeNow() {
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}

class AModule : public QObject {
  Q_OBJECT

 protected:
  static constexpr std::string_view kTopic = "robocin";

 public:
  explicit AModule(int id, float sleep_duration_ms);
  void Run();

  void MockedSleep();

  template <class Sender, class Receiver>
  void Connect(AModule& prev) {
    connect(&prev, &Sender::QtSendMessage, this, &Receiver::ReceiveMessage, Qt::DirectConnection);
  }

 public:
  const int id_;

  virtual void ParallelRun();
  virtual void ISendMessage(const ::robocin::ZmqDatagram& message);

  std::string MakeNextMessage(std::string_view message);

  std::mutex mutex_;
  std::condition_variable cv_;
  std::vector<::robocin::ZmqDatagram> shared_packages_;

 private:
  float sleep_duration_ms_ = 0.0F;

  int serial_id_ = 0;
  int field_serial_id_ = 0;

  std::unique_ptr<std::jthread> thread_;

 signals:
  void QtSendMessage(const ::robocin::ZmqDatagram& message);

 private slots:
  void ReceiveMessage(const ::robocin::ZmqDatagram& message);
};

} // namespace vision::qt_monolithic

#endif // VISION_QT_MONOLITHIC_UTILITY_H
