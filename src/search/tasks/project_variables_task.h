#ifndef TASKS_PROJECT_VARIABLES_TASK_H
#define TASKS_PROJECT_VARIABLES_TASK_H

#include "delegating_task.h"

#include "../operator_cost.h"

namespace plugins {
class Options;
}

namespace tasks {
/*
  Task transformation that projects away some of the variables in the input*/
class VariableProjectedTask : public DelegatingTask {
    const std::vector<int> variables_to_keep; // these are the FDR variables that should be kept, i.e., all others are projected away
public:
    VariableProjectedTask(
        const std::shared_ptr<AbstractTask> &parent,
		std::vector<int> _variables_to_keep);
    virtual ~VariableProjectedTask() override = default;

    virtual int get_operator_cost(int index, bool is_axiom) const override;
};
}

#endif
