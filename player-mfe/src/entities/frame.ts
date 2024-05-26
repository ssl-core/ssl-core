import { Ball } from "./ball";
import { Field } from "./field";
import { Robot } from "./robot";

export type Frame = {
  serial_id: number;
  effective_serial_id: number;
  fps: number;
  balls: Ball[];
  robots: Robot[];
  field: Field;
  created_at: string;
};
