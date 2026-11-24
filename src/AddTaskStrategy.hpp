#pragma once

#include "Command.hpp"
#include "Lift.hpp"

class AddTaskStrategy {
protected:
  Lift &lift;
  Command &command;

public:
  AddTaskStrategy(Lift &lift, Command &command)
      : lift(lift), command(command) {}

  virtual ~AddTaskStrategy() = default;

  virtual bool can_add_task() const = 0;
  virtual void add_task() = 0;
};

class LiftHasNoTasks_Higher_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::NoTasks &&
           command.get_floor() > lift.get_current_floor();
  }

  void add_task() override {
    lift.push_back_to_tasks_higher(command.get_floor());
    lift.set_state(Direction::TargetIsHigher);
  }
};

class LiftHasNoTasks_Lower_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::NoTasks &&
           command.get_floor() < lift.get_current_floor();
  }

  void add_task() override {
    lift.push_back_to_tasks_lower(command.get_floor());
    lift.set_state(Direction::TargetIsLower);
  }
};

class LiftHasNoTasks_Current_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::NoTasks &&
           command.get_floor() == lift.get_current_floor();
  }

  void add_task() override { lift.stopping_message(); }
};

class LiftGoesUp_Current_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsHigher &&
           command.get_floor() == lift.get_current_floor();
  }

  void add_task() override { lift.add_to_tasks_lower(command.get_floor()); }
};

class LiftGoesUp_Higher_Cabin_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsHigher &&
           command.get_type() == CommandType::InCabin &&
           command.get_floor() > lift.get_current_floor();
  }

  void add_task() override { lift.add_to_tasks_higher(command.get_floor()); }
};

class LiftGoesUp_Lower_Cabin_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsHigher &&
           command.get_type() == CommandType::InCabin &&
           command.get_floor() < lift.get_current_floor();
  }

  void add_task() override { lift.add_to_tasks_lower(command.get_floor()); }
};

class LiftGoesUp_HigherTopTask_OnFloor_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsHigher &&
           command.get_type() == CommandType::OnFloor &&
           command.get_floor() > lift.get_current_floor() &&
           lift.has_no_tasks_higher_than(command);
  }

  void add_task() override {
    lift.push_back_to_tasks_higher(command.get_floor());
  }
};

class LiftGoesUp_LowerTopTask_OnFloor_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsHigher &&
           command.get_type() == CommandType::OnFloor &&
           command.get_floor() > lift.get_current_floor() &&
           !lift.has_no_tasks_higher_than(command);
  }

  void add_task() override { lift.add_to_tasks_lower(command.get_floor()); }
};

class LiftGoesUp_Lower_OnFloor_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsHigher &&
           command.get_type() == CommandType::OnFloor &&
           command.get_floor() < lift.get_current_floor();
  }

  void add_task() override { lift.add_to_tasks_lower(command.get_floor()); }
};

class LiftGoesDown_Current_Cabin_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsLower &&
           command.get_type() == CommandType::InCabin &&
           command.get_floor() == lift.get_current_floor();
  }

  void add_task() override { lift.add_to_tasks_higher(command.get_floor()); }
};

class LiftGoesDown_Higher_Cabin_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsLower &&
           command.get_type() == CommandType::InCabin &&
           command.get_floor() > lift.get_current_floor();
  }

  void add_task() override { lift.add_to_tasks_higher(command.get_floor()); }
};

class LiftGoesDown_Lower_Cabin_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsLower &&
           command.get_type() == CommandType::InCabin &&
           command.get_floor() < lift.get_current_floor();
  }

  void add_task() override { lift.add_to_tasks_lower(command.get_floor()); }
};

class LiftGoesDown_Current_OnFloor_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsLower &&
           command.get_type() == CommandType::OnFloor &&
           command.get_floor() == lift.get_current_floor();
  }

  void add_task() override {
    lift.push_back_to_tasks_lower(command.get_floor());
  }
};

class LiftGoesDown_Higher_OnFloor_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsLower &&
           command.get_type() == CommandType::OnFloor &&
           command.get_floor() > lift.get_current_floor();
  }

  void add_task() override { lift.add_to_tasks_higher(command.get_floor()); }
};

class LiftGoesDown_Lower_OnFloor_Strategy : public AddTaskStrategy {
public:
  using AddTaskStrategy::AddTaskStrategy;

  bool can_add_task() const override {
    return lift.get_state() == Direction::TargetIsLower &&
           command.get_type() == CommandType::OnFloor &&
           command.get_floor() < lift.get_current_floor();
  }

  void add_task() override { lift.add_to_tasks_lower(command.get_floor()); }
};