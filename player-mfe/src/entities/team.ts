import { TeamResponse } from "../types/requests";

class Team {
  private name: string;
  private score: number;
  private isPositiveHalf: boolean;
  private color: RobotColor;
  private goalkeeperId: number;
  private yellowCards: number;
  private timeToExpireActiveYellowCards: number[];
  private redCards: number;
  private timeoutsLeft: number;
  private totalTimeoutTimeLeft: Date;
  private foulsCommitted: number;
  private consecutiveBallPlacementFailures: number;
  private isBallPlacementEnabled: boolean;
  private hasBallPlacementFailuresReachedMaximum: boolean;
  private maximumAllowedRobots: number;
  private isRobotSubstitutionRequested: boolean;
  private isRobotSubstitutionAllowed: boolean;
  private robotSubstitutionsLeft: number;
  private robotSubstitutionTimeLeft: Date;

  constructor(data: TeamResponse) {
    this.name = data.name;
    this.score = data.score;
    this.isPositiveHalf = data.is_positive_half;
    this.color = data.robot_color;
    this.goalkeeperId = data.goalkeeper_id;
    this.yellowCards = data.yellow_cards;
    this.timeToExpireActiveYellowCards =
      data.time_to_expire_active_yellow_cards;
    this.redCards = data.red_cards;
    this.timeoutsLeft = data.timeouts_left;
    this.totalTimeoutTimeLeft = new Date(data.total_timeout_time_left);
    this.foulsCommitted = data.fouls_committed;
    this.consecutiveBallPlacementFailures =
      data.consecutive_ball_placement_failures;
    this.isBallPlacementEnabled = data.is_ball_placement_enabled;
    this.hasBallPlacementFailuresReachedMaximum =
      data.has_ball_placement_failures_reached_maximum;
    this.maximumAllowedRobots = data.maximum_allowed_robots;
    this.isRobotSubstitutionRequested = data.is_robot_substitution_requested;
    this.isRobotSubstitutionAllowed = data.is_robot_substitution_allowed;
    this.robotSubstitutionsLeft = data.robot_substitutions_left;
    this.robotSubstitutionTimeLeft = new Date(
      data.robot_substitution_time_left
    );
  }
}

export default Team;
