#include "project_variables_task.h"

//#include "../operator_cost.h"

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
		  for (int v : _variables_to_keep){
			  int newVarID = parent_vars_to_my_vars.size();
			  utils::g_log << "VAR: " << v << " becomes " << newVarID << endl;
			  parent_vars_to_my_vars[v] = newVarID;
		  }
}

int VariableProjectedTask::get_num_variables() const {
    //return parent->get_num_variables();
    return variables_to_keep.size();
}

string VariableProjectedTask::get_variable_name(int var) const {
    return parent->get_variable_name(variables_to_keep[var]);
}

int VariableProjectedTask::get_variable_domain_size(int var) const {
    return parent->get_variable_domain_size(variables_to_keep[var]);
}

int VariableProjectedTask::get_variable_axiom_layer(int var) const {
    return parent->get_variable_axiom_layer(variables_to_keep[var]);
}

int VariableProjectedTask::get_variable_default_axiom_value(int var) const {
    return parent->get_variable_default_axiom_value(variables_to_keep[var]);
}

string VariableProjectedTask::get_fact_name(const FactPair &fact) const {
    return parent->get_fact_name(fact);
}

bool VariableProjectedTask::are_facts_mutex(const FactPair &fact1, const FactPair &fact2) const {
    return parent->are_facts_mutex(fact1, fact2);
}

int VariableProjectedTask::get_num_operator_preconditions(int index, bool is_axiom) const {
    int num_parent_preconditions = parent->get_num_operator_preconditions(index, is_axiom);
	int num_me = 0;
	for (int p = 0; p < num_parent_preconditions; p++){
		FactPair pre = parent->get_operator_precondition(index,p,is_axiom);
		if (parent_vars_to_my_vars.count(pre.var)) num_me++;
	}

	return num_me;
}

FactPair VariableProjectedTask::get_operator_precondition(
    int op_index, int fact_index, bool is_axiom) const {

	int num_parent_preconditions = parent->get_num_operator_preconditions(op_index, is_axiom);
	int num_me = 0;
	for (int p = 0; p < num_parent_preconditions; p++){
		FactPair pre = parent->get_operator_precondition(op_index,p,is_axiom);
		auto it = parent_vars_to_my_vars.find(pre.var);
		if (it != parent_vars_to_my_vars.end()) {
			if (fact_index == num_me){
				int newVariable = it->second; 
				FactPair ret(newVariable,pre.value);
			}
			num_me++;
		}
	}
	assert(false);
	return FactPair(-1,-1);
}

int VariableProjectedTask::get_num_operator_effects(int op_index, bool is_axiom) const {
    int num_parent_effects = parent->get_num_operator_effects(op_index, is_axiom);
	int num_me = 0;
	for (int e = 0; e < num_parent_effects; e++){
		FactPair eff = parent->get_operator_effect(op_index,e,is_axiom);
		if (parent_vars_to_my_vars.count(eff.var)) num_me++;
	}

	return num_me;
}

FactPair VariableProjectedTask::get_operator_effect(
    int op_index, int eff_index, bool is_axiom) const {
   
   	int num_parent_effects = parent->get_num_operator_effects(op_index, is_axiom);
	int num_me = 0;
	for (int e = 0; e < num_parent_effects; e++){
		FactPair eff = parent->get_operator_effect(op_index,e,is_axiom);
		if (parent_vars_to_my_vars.count(eff.var)){
			if (eff_index == num_me){
				int newVariable = parent_vars_to_my_vars.at(eff.var);
				FactPair ret(newVariable,eff.value);
				return ret;
			}
			num_me++;
		}
	}
	assert(false);
	return FactPair(-1,-1);
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
