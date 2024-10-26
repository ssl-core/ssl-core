#ifndef BEHAVIOR_PROCESSING_MESSAGES_COMMON_ROBOT_ID_H
#define BEHAVIOR_PROCESSING_MESSAGES_COMMON_ROBOT_ID_H

#include <robocin/utility/iproto_convertible.h>

#include <protocols/common/robot_id.pb.h>

namespace behavior {

class RobotIdMessage : public robocin::IProtoConvertible<protocols::common::RobotId> {
    enum Color {
        COLOR_UNSPECIFIED = 0,
        COLOR_YELLOW = 1,
        COLOR_BLUE = 2,
    };

  public:
    explicit RobotIdMessage(std::optional<Color> color = std::nullopt,
                            uint32_t number = 0);

    [[nodiscard]] protocols::common::RobotId toProto() const override {
        protocols::common::RobotId robot_id;
        robot_id.set_color(static_cast<::protocols::common::RobotId_Color>(color.value_or(Color::COLOR_UNSPECIFIED)));
        robot_id.set_number(number);
    
        return robot_id;
    };

    void fromProto(protocols::common::RobotId robot_id) override {
        color = static_cast<Color>(robot_id.color());
        number = robot_id.number();
    };

    std::optional<Color> color;
    uint32_t number;
};

} // namespace behavior

#endif // BEHAVIOR_PROCESSING_MESSAGES_COMMON_ROBOT_ID_H
