import json

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
        return json.dumps(self.to_dict())
    
    def from_string(field_string):
        field_dict = json.loads(field_string)

        length = field_dict['length']
        width = field_dict['width']
        goal_depth = field_dict['goal_depth']
        goal_width = field_dict['goal_width']
        penalty_area_depth = field_dict['penalty_area_depth']
        penalty_area_width = field_dict['penalty_area_width']
        boundary_width = field_dict['boundary_width']
        goal_center_to_penalty_mark = field_dict['goal_center_to_penalty_mark']

        return Field(length, width, goal_depth, goal_width, penalty_area_depth, penalty_area_width, boundary_width, goal_center_to_penalty_mark)
    
    @staticmethod
    def from_dict(field_dict):
        length = field_dict['length']
        width = field_dict['width']
        goal_depth = field_dict['goal_depth']
        goal_width = field_dict['goal_width']
        penalty_area_depth = field_dict['penalty_area_depth']
        penalty_area_width = field_dict['penalty_area_width']
        boundary_width = field_dict['boundary_width']
        goal_center_to_penalty_mark = field_dict['goal_center_to_penalty_mark']
        return Field(length, width, goal_depth, goal_width, penalty_area_depth, penalty_area_width, boundary_width, goal_center_to_penalty_mark)

    def to_dict(self):
        return {
            "length": self.length,
            "width": self.width,
            "goal_depth": self.goal_depth,
            "goal_width": self.goal_width,
            "penalty_area_depth": self.penalty_area_depth,
            "penalty_area_width": self.penalty_area_width,
            "boundary_width": self.boundary_width,
            "goal_center_to_penalty_mark": self.goal_center_to_penalty_mark
        }
