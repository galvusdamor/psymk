#include "project_variables_task.h"

#include "../operator_cost.h"

#include "../plugins/plugin.h"
#include "../tasks/root_task.h"

#include "../utils/logging.h"

#include <iostream>
#include <memory>

using namespace std;
using utils::ExitCode;

namespace tasks {
VariableProjectedTask::VariableProjectedTask(
    const shared_ptr<AbstractTask> &parent,
    std::vector<int> _variables_to_keep)
    : DelegatingTask(parent),
      variables_to_keep(_variables_to_keep) {
		  utils::g_log << "This is a test" << endl;
		  for (int v : _variables_to_keep)
			  utils::g_log << "VAR: " << v << endl;
		  exit(-1);
}

int VariableProjectedTask::get_num_variables() const {
    return parent->get_num_variables();
}

string VariableProjectedTask::get_variable_name(int var) const {
    return parent->get_variable_name(var);
}

int VariableProjectedTask::get_variable_domain_size(int var) const {
    return parent->get_variable_domain_size(var);
}

int VariableProjectedTask::get_variable_axiom_layer(int var) const {
    return parent->get_variable_axiom_layer(var);
}

int VariableProjectedTask::get_variable_default_axiom_value(int var) const {
    return parent->get_variable_default_axiom_value(var);
}

string VariableProjectedTask::get_fact_name(const FactPair &fact) const {
    return parent->get_fact_name(fact);
}

bool VariableProjectedTask::are_facts_mutex(const FactPair &fact1, const FactPair &fact2) const {
    return parent->are_facts_mutex(fact1, fact2);
}

int VariableProjectedTask::get_num_operator_preconditions(int index, bool is_axiom) const {
    return parent->get_num_operator_preconditions(index, is_axiom);
}

FactPair VariableProjectedTask::get_operator_precondition(
    int op_index, int fact_index, bool is_axiom) const {
    return parent->get_operator_precondition(op_index, fact_index, is_axiom);
}

int VariableProjectedTask::get_num_operator_effects(int op_index, bool is_axiom) const {
    return parent->get_num_operator_effects(op_index, is_axiom);
}

FactPair VariableProjectedTask::get_operator_effect(
    int op_index, int eff_index, bool is_axiom) const {
    return parent->get_operator_effect(op_index, eff_index, is_axiom);
}

class VariableProjectedTaskFeature : public plugins::TypedFeature<AbstractTask, VariableProjectedTask> {
public:
    VariableProjectedTaskFeature() : TypedFeature("project_variables") {
        document_title("Project the task onto a set of variables");
        document_synopsis(
            "Projects the task onto a subset of the variables. Variables are given as a list");

		add_list_option<int>(
            "variables",
            "variables to project onto (the ones to be kept)", "[]");
        //add_cost_type_option_to_feature(*this);
    }

    virtual shared_ptr<VariableProjectedTask> create_component(const plugins::Options &options, const utils::Context &) const override {
		std::vector<int> vars = options.get_list<int>("variables");
        return make_shared<VariableProjectedTask>(g_root_task, vars);
    }
};

static plugins::FeaturePlugin<VariableProjectedTaskFeature> _plugin;
}
