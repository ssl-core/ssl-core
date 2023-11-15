class Field:
    def __init__(self,
                 length,
                 width,
                 goal_depth,
                 goal_width,
                 penalty_area_depth,
                 penalty_area_width,
                 boundary_width,
                 goal_center_to_penalty_mark):
        self.length = length
        self.width = width
        self.goal_depth = goal_depth
        self.goal_width = goal_width
        self.penalty_area_depth = penalty_area_depth
        self.penalty_area_width = penalty_area_width
        self.boundary_width = boundary_width
        self.goal_center_to_penalty_mark = goal_center_to_penalty_mark

    def __str__(self):
        return f'Field(length={self.length}, width={self.width}, goal_depth={self.goal_depth}, goal_width={self.goal_width}, penalty_area_depth={self.penalty_area_depth}, penalty_area_width={self.penalty_area_width}, boundary_width={self.boundary_width}, goal_center_to_penalty_mark={self.goal_center_to_penalty_mark})'