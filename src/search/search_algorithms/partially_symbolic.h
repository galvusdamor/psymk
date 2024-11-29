#ifndef SEARCH_ALGORITHMS_P_SYM_SEARCH_H
#define SEARCH_ALGORITHMS_P_SYM_SEARCH_H

#include "../evaluation_context.h"
#include "../evaluator.h"
#include "../open_list.h"
#include "../operator_id.h"
#include "../search_algorithm.h"
#include "../search_progress.h"
#include "../search_space.h"

#include "../symbolic/sym_variables.h"
#include "../symbolic/sym_parameters.h"
#include "../symbolic/sym_state_space_manager.h"

#include "../utils/rng.h"

#include <memory>
#include <vector>

namespace partially_symbolic {
class PartiallySymbolicSearch : public SearchAlgorithm {
protected:
	symbolic::SymParameters sym_params; // Parameters for symbolic search
	std::shared_ptr<symbolic::SymVariables> vars; // The symbolic variables are declared
	std::shared_ptr<symbolic::SymStateSpaceManager> mgr; // Symbolic manager to perform bdd operations 

	std::shared_ptr<Evaluator> evaluator;
    //std::unique_ptr<EdgeOpenList> open_list;

    // Search behavior parameters
    bool reopen_closed_nodes; // whether to reopen closed nodes upon finding lower g paths
    //bool randomize_successors;
    //bool preferred_successors_first;
    //std::shared_ptr<utils::RandomNumberGenerator> rng;

    //std::vector<Evaluator *> path_dependent_evaluators;
    //std::vector<std::shared_ptr<Evaluator>> preferred_operator_evaluators;

    State current_state;
    StateID current_predecessor_id;
    OperatorID current_operator_id;
    int current_g;
    int current_real_g;
    EvaluationContext current_eval_context;

    virtual void initialize() override;
    virtual SearchStatus step() override;

    void generate_successors();
    SearchStatus fetch_next_state();

    std::vector<OperatorID> get_successor_operators() const;

public:
    explicit PartiallySymbolicSearch(const plugins::Options &opts);
    virtual ~PartiallySymbolicSearch() = default;

    //void set_preferred_operator_evaluators(std::vector<std::shared_ptr<Evaluator>> &evaluators);

    virtual void print_statistics() const override;
};
}

#endif
