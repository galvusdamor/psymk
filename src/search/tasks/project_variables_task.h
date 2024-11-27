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

    virtual int get_num_variables() const override;
    virtual std::string get_variable_name(int var) const override;
    virtual int get_variable_domain_size(int var) const override;
    virtual int get_variable_axiom_layer(int var) const override;
    virtual int get_variable_default_axiom_value(int var) const override;
    virtual std::string get_fact_name(const FactPair &fact) const override;
    virtual bool are_facts_mutex(const FactPair &fact1, const FactPair &fact2) const override;

	virtual int get_num_operator_preconditions(int index, bool is_axiom) const override;
    virtual FactPair get_operator_precondition(int op_index, int fact_index, bool is_axiom) const override;
    virtual int get_num_operator_effects(int op_index, bool is_axiom) const override;
    virtual FactPair get_operator_effect(int op_index, int eff_index, bool is_axiom) const override;
    
	// needed only for conditional effects
	//virtual int get_num_operator_effect_conditions(int op_index, int eff_index, bool is_axiom) const override;
    //virtual FactPair get_operator_effect_condition(int op_index, int eff_index, int cond_index, bool is_axiom) const override;

};
}

#endif
