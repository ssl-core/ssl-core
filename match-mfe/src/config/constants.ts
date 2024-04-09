const constants = {
  background: "#1B1C1F",
  epsilon: 0.001,
  clippingEpsilon: 0.01,
  numRobots: 22,
  numBalls: 1,
  field: {
    surface: {
      color: "#000000",
    },
    line: {
      width: 0.02,
      color: "#ffffff",
    },
    center: {
      radius: 0.5,
      segments: 48,
    },
    goal: {
      height: 0.16,
      thickness: 0.02,
      color: "#FFFFFF",
    },
    wall: {
      height: 0.18,
      thickness: 0.02,
      color: "#000000",
    },
    color: "#224725",
  },
  robot: {
    chassis: {
      radius: 0.09,
      height: 0.147,
      segments: 48,
      color: "#1a1a1a",
      bottomHeight: 0.02,
    },
    wheels: {
      radius: 0.027,
      thickness: 0.005,
      segments: 48,
      color: "#e5e5e5",
      angles: [60, 135, 225, 300],
    },
    dots: {
      center: {
        radius: 0.025,
      },
      other: {
        radius: 0.02,
        minDistance: 0.035,
        maxDistance: 0.054772,
      },
      common: {
        segments: 48,
        z: 0.0735,
      },
      colors: {
        blue: "#0000FF",
        yellow: "#FFFF00",
        patterns: [
          ["#FF00FF", "#FF00FF", "#00FF00", "#FF00FF"],
          ["#00FF00", "#FF00FF", "#00FF00", "#FF00FF"],
          ["#00FF00", "#00FF00", "#00FF00", "#FF00FF"],
          ["#FF00FF", "#00FF00", "#00FF00", "#FF00FF"],
          ["#FF00FF", "#FF00FF", "#FF00FF", "#00FF00"],
          ["#00FF00", "#FF00FF", "#FF00FF", "#00FF00"],
          ["#00FF00", "#00FF00", "#FF00FF", "#00FF00"],
          ["#FF00FF", "#00FF00", "#FF00FF", "#00FF00"],
          ["#00FF00", "#00FF00", "#00FF00", "#00FF00"],
          ["#FF00FF", "#FF00FF", "#FF00FF", "#FF00FF"],
          ["#FF00FF", "#FF00FF", "#00FF00", "#00FF00"],
          ["#00FF00", "#00FF00", "#FF00FF", "#FF00FF"],
          ["#00FF00", "#FF00FF", "#00FF00", "#00FF00"],
          ["#00FF00", "#FF00FF", "#FF00FF", "#FF00FF"],
          ["#FF00FF", "#00FF00", "#00FF00", "#00FF00"],
          ["#FF00FF", "#00FF00", "#FF00FF", "#FF00FF"],
        ],
      },
    },
  },
  ball: {
    radius: 0.043,
    color: "#D9650A",
    segments: 32,
  },
};

export default constants;
