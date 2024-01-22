import random
from src.business.Point.Point2Df import Point2Df
from src.business.Point.Point3Df import Point3Df
from src.business.Robot.Robot import Robot
from src.business.Robot.Color import Color
from src.business.Ball.Ball import Ball
from src.business.Field.Field import Field
from src.business.Frame.Frame import Frame
from src.business.Source.Source import Source

# Create a default field.
default_field = Field(length=9000.0, width=6000.0, goal_depth=500.0, goal_width=1000.0,
                      penalty_area_depth=1000.0, penalty_area_width=2000.0, boundary_width=20.0,
                      goal_center_to_penalty_mark=6000.0)

# Create a list of frames with different robots and the same ball and field.
frames_with_different_robots = []

# Create # of frames
NUMBER_OF_FRAMES = 100000
for frame_id in range(NUMBER_OF_FRAMES):
    create_robot = lambda color, id: Robot(
        uuid=str(id),
        color=color,
        id=id,
        position=Point2Df(x=random.uniform(0.0, default_field.length / 2), y=random.uniform(0.0, default_field.width / 2)),
        angle=0.0,
        velocity=Point2Df(x=0.0, y=0.0),
        angular_velocity=0.0,
        source=Source.SIMULATION
    )

    # Create the blue robots.
    blue_robots = [create_robot(Color.BLUE, id) for id in range(1, 12)]

    # Create the yellow robots.
    yellow_robots = [create_robot(Color.YELLOW, id) for id in range(1, 12)]

    # Combine blue and yellow robots.
    all_robots = blue_robots + yellow_robots

    # Create a ball.
    default_ball = Ball(uuid="ball",
                        position=Point3Df(x=50.0, y=25.0, z=0.0),
                        velocity=Point3Df(x=0.0, y=0.0, z=0.0),
                        source=Source.SIMULATION)

    # Create a frame with the generated robots, the same ball, and the default field.
    frame = Frame(id=str(frame_id), balls=[default_ball], robots=all_robots, field=default_field)

    # Add the frame to the list.
    frames_with_different_robots.append(frame)
