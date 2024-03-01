const constants = {
  robot: {
    chassis: {
      radius: 0.09,
      height: 0.147,
      segments: 48,
      color: "#1a1a1a",
      bottom_height: 0.02,
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
        min_distance: 0.035,
        max_distance: 0.054772,
      },
      common: {
        segments: 48,
        y: 0.0735,
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
  field: {
    surface: {
      margin: 0.4,
      color: "#000000",
    },
    outside: {
      margin: 0.3,
    },
    line: {
      width: 0.02,
      color: "#ffffff",
    },
    goal: {
      width: 0.2,
      height: 1.8,
      color: "#0000FF",
    },
    penalty: {
      width: 1.8,
      height: 3.6,
      color: "#FF0000",
    },
    center: {
      radius: 0.5,
    },
    width: 12,
    height: 9,
    segments: 32,
    color: "#224725",
  },
  goal: {
    thickness: 0.02,
    color: "#FFFFFF",
    width: 1.8,
    depth: 0.18,
    height: 0.16,
  },
  ball: {
    radius: 0.043,
    color: "#D9650A",
    segments: 32,
  },
};

export default constants;
