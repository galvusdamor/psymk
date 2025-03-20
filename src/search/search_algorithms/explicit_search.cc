#include "explicit_search.h"

#include "../open_list_factory.h"

#include "../algorithms/ordered_set.h"
#include "../plugins/options.h"
#include "../task_utils/successor_generator.h"
#include "../task_utils/task_properties.h"
#include "../utils/logging.h"
#include "../utils/rng.h"
#include "../utils/rng_options.h"

#include <algorithm>
#include <limits>
#include <vector>

using namespace std;

namespace explicit_search {
ExplicitSearch::ExplicitSearch(const plugins::Options &opts)
    : SearchAlgorithm(opts),
	  evaluator(opts.get<shared_ptr<Evaluator>>("eval")),
      reopen_closed_nodes(opts.get<bool>("reopen_closed")),
      //open_list(opts.get<shared_ptr<OpenListFactory>>("open")->
      //          create_edge_open_list()),
      //randomize_successors(opts.get<bool>("randomize_successors")),
      //preferred_successors_first(opts.get<bool>("preferred_successors_first")),
      //rng(utils::parse_rng_from_options(opts)),
      current_state(state_registry.get_initial_state()),
      current_predecessor_id(StateID::no_state),
      current_operator_id(OperatorID::no_operator),
      current_g(0),
      current_real_g(0),
      current_eval_context(current_state, 0, true, &statistics) {
    cerr << "Second Hello!" <<endl;
	if (has_sdac_cost) {
        cerr << "error: explicit search does not support state-dependent action costs. "
             << "Please use symbolic search." << endl;
        utils::exit_with(utils::ExitCode::SEARCH_INPUT_ERROR);
    }
}

//void PartiallySymbolicSearch::set_preferred_operator_evaluators(
//    vector<shared_ptr<Evaluator>> &evaluators) {
//    preferred_operator_evaluators = evaluators;
//}

void ExplicitSearch::initialize() {
    log << "Conducting Partially Symbolic Search, (real) bound = " << bound << endl;

    //assert(open_list);
    State initial_state = state_registry.get_initial_state();
	EvaluationContext new_eval_context(initial_state, 0, true, nullptr);
	EvaluationResult result = evaluator->compute_result(new_eval_context);
	log << "Initial heuristic value: " << result.get_evaluator_value() << endl;
}

vector<OperatorID> ExplicitSearch::get_successor_operators() const {
    vector<OperatorID> applicable_operators;
    successor_generator.generate_applicable_ops(current_state, applicable_operators);
    return applicable_operators;
}

void ExplicitSearch::generate_successors() {

    vector<OperatorID> successor_operators = get_successor_operators();
    statistics.inc_generated(successor_operators.size());

    for (OperatorID op_id : successor_operators) {
        OperatorProxy op = task_proxy.get_operators()[op_id];
        int new_g = current_g + get_adjusted_cost(op);
        int new_real_g = current_real_g + op.get_cost();

		// compute the successor state and set the "current state" to be this successor state
    	OperatorProxy current_operator = task_proxy.get_operators()[op_id];
		current_state = state_registry.get_successor_state(current_state, current_operator); 
        
        //if (new_real_g < bound) {
        //    EvaluationContext new_eval_context(
        //        current_eval_context, new_g, true, nullptr);
        //    open_list->insert(new_eval_context, make_pair(current_state.get_id(), op_id));
        //}
    }
}

SearchStatus ExplicitSearch::fetch_next_state() {
    //if (open_list->empty()) {
    //    log << "Completely explored state space -- no solution!" << endl;
    //    return FAILED;
    //}

    //EdgeOpenListEntry next = open_list->remove_min();

    //current_predecessor_id = next.first;
    //current_operator_id = next.second;
    //State current_predecessor = state_registry.lookup_state(current_predecessor_id);
    //OperatorProxy current_operator = task_proxy.get_operators()[current_operator_id];
    //assert(task_properties::is_applicable(current_operator, current_predecessor));
    //current_state = state_registry.get_successor_state(current_predecessor, current_operator);

    //SearchNode pred_node = search_space.get_node(current_predecessor);
    //current_g = pred_node.get_g() + get_adjusted_cost(current_operator);
    //current_real_g = pred_node.get_real_g() + current_operator.get_cost();

    ///*
    //  Note: We mark the node in current_eval_context as "preferred"
    //  here. This probably doesn't matter much either way because the
    //  node has already been selected for expansion, but eventually we
    //  should think more deeply about which path information to
    //  associate with the expanded vs. evaluated nodes in lazy search
    //  and where to obtain it from.
    //*/
    //current_eval_context = EvaluationContext(current_state, current_g, true, &statistics);

    return IN_PROGRESS;
}

SearchStatus ExplicitSearch::step() {
	log << "Performing a step" << endl;
    return FAILED;
	return fetch_next_state();
    // Invariants:
    // - current_state is the next state for which we want to compute the heuristic.
    // - current_predecessor is a permanent pointer to the predecessor of that state.
    // - current_operator is the operator which leads to current_state from predecessor.
    // - current_g is the g value of the current state according to the cost_type
    // - current_real_g is the g value of the current state (using real costs)


    //SearchNode node = search_space.get_node(current_state);
    //bool reopen = reopen_closed_nodes && !node.is_new() &&
    //    !node.is_dead_end() && (current_g < node.get_g());

    //if (node.is_new() || reopen) {
    //    if (current_operator_id != OperatorID::no_operator) {
    //        assert(current_predecessor_id != StateID::no_state);
    //        if (!path_dependent_evaluators.empty()) {
    //            State parent_state = state_registry.lookup_state(current_predecessor_id);
    //            for (Evaluator *evaluator : path_dependent_evaluators)
    //                evaluator->notify_state_transition(
    //                    parent_state, current_operator_id, current_state);
    //        }
    //    }
    //    statistics.inc_evaluated_states();
    //    if (!open_list->is_dead_end(current_eval_context)) {
    //        // TODO: Generalize code for using multiple evaluators.
    //        if (current_predecessor_id == StateID::no_state) {
    //            node.open_initial();
    //            if (search_progress.check_progress(current_eval_context))
    //                statistics.print_checkpoint_line(current_g);
    //        } else {
    //            State parent_state = state_registry.lookup_state(current_predecessor_id);
    //            SearchNode parent_node = search_space.get_node(parent_state);
    //            OperatorProxy current_operator = task_proxy.get_operators()[current_operator_id];
    //            if (reopen) {
    //                node.reopen(parent_node, current_operator, get_adjusted_cost(current_operator));
    //                statistics.inc_reopened();
    //            } else {
    //                node.open(parent_node, current_operator, get_adjusted_cost(current_operator));
    //            }
    //        }
    //        node.close();
    //        if (check_goal_and_set_plan(current_state))
    //            return SOLVED;
    //        if (search_progress.check_progress(current_eval_context)) {
    //            statistics.print_checkpoint_line(current_g);
    //            reward_progress();
    //        }
    //        generate_successors();
    //        statistics.inc_expanded();
    //    } else {
    //        node.mark_as_dead_end();
    //        statistics.inc_dead_ends();
    //    }
    //    if (current_predecessor_id == StateID::no_state) {
    //        print_initial_evaluator_values(current_eval_context);
    //    }
    //}
}

void ExplicitSearch::print_statistics() const {
    statistics.print_detailed_statistics();
    search_space.print_statistics();
}
}
