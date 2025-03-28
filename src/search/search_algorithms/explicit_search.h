#ifndef SEARCH_ALGORITHMS_EXP_SEARCH_H
#define SEARCH_ALGORITHMS_EXP_SEARCH_H

#include "../evaluation_context.h"
#include "../evaluator.h"
#include "../open_list.h"
#include "../operator_id.h"
#include "../search_algorithm.h"
#include "../search_progress.h"
#include "../search_space.h"


#include "../utils/rng.h"

#include <memory>
#include <vector>

namespace explicit_search {
class ExplicitSearch : public SearchAlgorithm {
protected:
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
    explicit ExplicitSearch(const plugins::Options &opts);
    virtual ~ExplicitSearch() = default;

    //void set_preferred_operator_evaluators(std::vector<std::shared_ptr<Evaluator>> &evaluators);

    virtual void print_statistics() const override;
};
}

#endif
