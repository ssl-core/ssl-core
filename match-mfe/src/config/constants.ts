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
    },
  },
  field: {
    width: 5,
    height: 9,
    segments: 32,
    color: "#037a41",
  },
};

export default constants;
