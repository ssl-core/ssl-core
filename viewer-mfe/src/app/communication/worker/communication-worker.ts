import CommunicationManagerFactory from "../communication-manager-factory";
import CommunicationEventHandler from "./communication-event-handler";

const communicationManager =
  CommunicationManagerFactory.createManager("eventBus");
const handler = new CommunicationEventHandler(communicationManager);

self.onmessage = (event) => {
  const { data } = event;
  handler.handleEvent(data.type, data.payload);
};

const channel = new BroadcastChannel("event-bus");
setInterval(() => {
  channel.postMessage(example);
}, 100);

const example = {
  type: "frame",
  payload: {
    serialId: 2381,
    startTime: "2024-07-10T02:50:34.000Z",
    currentTime: "2024-07-10T02:50:58.000Z",
    endTime: "2024-07-10T02:50:58.000Z",
    fps: 60,
    balls: [
      {
        confidence: 1,
        position: [-0.0008359575, -0.0009074639, 0],
        velocity: [-0.82094854, -3.8662863, 0],
      },
    ],
    robots: [
      {
        confidence: 0.9529478,
        robotId: 1,
        robotColor: "yellow",
        position: [-5.5191283, -4.309584],
        angle: -1.5735554,
        velocity: [0.0023349174, -0.0051738317],
        angularVelocity: -0.18512952,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 0,
        robotColor: "yellow",
        position: [-5.819989, -4.307544],
        angle: -1.5736108,
        velocity: [-0.008792732, 0.017554365],
        angularVelocity: -0.027169777,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 3,
        robotColor: "yellow",
        position: [-4.920093, -4.308806],
        angle: -1.5585867,
        velocity: [0.003989956, 0.0072574923],
        angularVelocity: 0.20096123,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 2,
        robotColor: "yellow",
        position: [-5.2193685, -4.308878],
        angle: -1.5676649,
        velocity: [-0.0015065776, -0.006225419],
        angularVelocity: -0.042028073,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 5,
        robotColor: "yellow",
        position: [-4.318858, -4.3079114],
        angle: -1.579781,
        velocity: [0.0068464885, 0.016078958],
        angularVelocity: -0.3867524,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 4,
        robotColor: "yellow",
        position: [-4.618918, -4.3083897],
        angle: -1.5587245,
        velocity: [0.009504777, 0.0131571805],
        angularVelocity: 0.23425429,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 7,
        robotColor: "yellow",
        position: [-3.717964, -4.3094473],
        angle: -1.5512382,
        velocity: [0.014331431, -0.0034561618],
        angularVelocity: 0.16194947,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 6,
        robotColor: "yellow",
        position: [-4.0199513, -4.30884],
        angle: -1.5532701,
        velocity: [0.0020842315, 0.008034098],
        angularVelocity: 0.21197619,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 9,
        robotColor: "yellow",
        position: [-3.1200757, -4.310367],
        angle: -1.5627648,
        velocity: [-0.004375362, -0.009057426],
        angularVelocity: 0.027855031,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 8,
        robotColor: "yellow",
        position: [-3.4195209, -4.309769],
        angle: -1.5587142,
        velocity: [-0.0058528543, 0.00001647785],
        angularVelocity: 0.06925227,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 10,
        robotColor: "blue",
        position: [2.8200417, -4.3105354],
        angle: -1.5745662,
        velocity: [-0.0008944852, -0.009769538],
        angularVelocity: -0.029149901,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 9,
        robotColor: "blue",
        position: [3.1211894, -4.3087025],
        angle: -1.5626603,
        velocity: [0.017296046, 0.008766207],
        angularVelocity: -0.0035748621,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 10,
        robotColor: "yellow",
        position: [-2.8194995, -4.3106856],
        angle: -1.5694301,
        velocity: [-0.0014358603, -0.008519642],
        angularVelocity: -0.21137604,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 8,
        robotColor: "blue",
        position: [3.4202936, -4.3105736],
        angle: -1.562209,
        velocity: [0.009734461, -0.0099270735],
        angularVelocity: 0.10502538,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 7,
        robotColor: "blue",
        position: [3.7210662, -4.3089128],
        angle: -1.552964,
        velocity: [0.003065455, 0.00381079],
        angularVelocity: 0.3824874,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 6,
        robotColor: "blue",
        position: [4.0194583, -4.3103237],
        angle: -1.5703603,
        velocity: [-0.0037867464, -0.0019209328],
        angularVelocity: -0.1825775,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 5,
        robotColor: "blue",
        position: [4.320965, -4.3110538],
        angle: -1.5640144,
        velocity: [0.0035135793, -0.0042087366],
        angularVelocity: 0.0050301217,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 4,
        robotColor: "blue",
        position: [4.62069, -4.310286],
        angle: -1.5566771,
        velocity: [0.01240523, -0.007224567],
        angularVelocity: 0.13114622,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 3,
        robotColor: "blue",
        position: [4.9198346, -4.309385],
        angle: -1.552484,
        velocity: [-0.0008176777, 0.001072358],
        angularVelocity: 0.019765608,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 2,
        robotColor: "blue",
        position: [5.2205496, -4.3086996],
        angle: -1.5697087,
        velocity: [0.004334635, 0.0030502316],
        angularVelocity: -0.033701032,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 1,
        robotColor: "blue",
        position: [5.520315, -4.308631],
        angle: -1.5563313,
        velocity: [-0.0116887195, 0.004426322],
        angularVelocity: 0.2009131,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
      {
        confidence: 0.9529478,
        robotId: 0,
        robotColor: "blue",
        position: [5.81985, -4.3091598],
        angle: -1.5534687,
        velocity: [-0.017007269, -0.0030357963],
        angularVelocity: 0.24247971,
        radius: 0.09,
        height: 0.14,
        dribblerWidth: 0.06,
      },
    ],
    field: {
      length: 0,
      width: 0,
      goalDepth: 0,
      goalWidth: 0,
      penaltyAreaDepth: 0,
      penaltyAreaWidth: 0,
      boundaryWidth: 0,
      goalCenterToPenaltyMark: 0,
    },
  },
};
