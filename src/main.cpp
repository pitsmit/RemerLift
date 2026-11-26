#include <csignal>
#include <iostream>
#include <poll.h>
#include <unistd.h>

#include "AddTaskStrategy.hpp"
#include "Command.hpp"
#include "Exceptions.hpp"
#include "Input.hpp"
#include "Lift.hpp"
#include "types.hpp"

class App {
  core::FloorNum count_floors = 0;
  static Lift lift;

  // выбор стратегии добавления задачи в список задач в зависимости от состояния
  // лифта и команды
  void process(Command &command) {
    std::vector<std::unique_ptr<AddTaskStrategy>> strategies;
    strategies.reserve(14);

    strategies.push_back(
        std::make_unique<LiftHasNoTasks_Higher_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftHasNoTasks_Lower_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftHasNoTasks_Current_Strategy>(lift, command));

    strategies.push_back(
        std::make_unique<LiftGoesUp_Current_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesUp_Higher_Cabin_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesUp_Lower_Cabin_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesUp_HigherTopTask_OnFloor_Strategy>(lift,
                                                                    command));
    strategies.push_back(
        std::make_unique<LiftGoesUp_LowerTopTask_OnFloor_Strategy>(lift,
                                                                   command));
    strategies.push_back(
        std::make_unique<LiftGoesUp_Lower_OnFloor_Strategy>(lift, command));

    strategies.push_back(
        std::make_unique<LiftGoesDown_Current_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Higher_Cabin_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Lower_Cabin_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Higher_OnFloor_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Lower_OnFloor_Strategy>(lift, command));

    for (const auto &strategy : strategies) {
      if (strategy->can_add_task()) {
        strategy->add_task();
        return;
      }
    }
    throw NoStrategyException("No suitable strategy found for this command.");
  }

  // получение команды и дальнейшая обработка
  void read_command() {
    std::string input;
    if (!std::getline(std::cin, input)) {
      return;
    }
    try {
      auto command = Command(input);
      if (command.has_valid_floor(count_floors)) {
        process(command);
      } else {
        throw InputException("Floor is out of range");
      }
    } catch (const InputException &e) {
      std::cout << e.what() << '\n';
    }
    lift.unmute();
  }

  static void handle_sigint(int) { lift.mute(); }

  static pollfd stdin_fd;
  static bool something_was_commanded() {
    return poll(&stdin_fd, 1, 1000) > 0 && (stdin_fd.revents & POLLIN);
  }

public:
  App() : count_floors(Input::floor_count()) {
    std::signal(SIGINT, handle_sigint);
  }

  // основной цикл работы приложения
  void run() {
    while (true) {
      if (something_was_commanded()) {
        read_command();
      }
      lift.next_step();
    }
  }
};

Lift App::lift{};
pollfd App::stdin_fd{.fd = STDIN_FILENO, .events = POLLIN, .revents = 0};

int main() {
  App().run();
  return 0;
}