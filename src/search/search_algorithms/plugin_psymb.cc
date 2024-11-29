#include "partially_symbolic.h"
#include "search_common.h"

#include "../plugins/plugin.h"

using namespace std;

namespace plugin_psymb {
class PSymbSearchFeature : public plugins::TypedFeature<SearchAlgorithm, partially_symbolic::PartiallySymbolicSearch> {
public:
    PSymbSearchFeature() : TypedFeature("psymb") {
        document_title("Partially Symbolic Search");
        document_synopsis("");
        
		add_option<shared_ptr<Evaluator>>("eval", "evaluator for h-value");
        add_option<bool>("reopen_closed", "reopen closed nodes", "false");

        //add_option<shared_ptr<OpenListFactory>>("open", "open list");
        //add_list_option<shared_ptr<Evaluator>>(
        //    "preferred",
        //    "use preferred operators of these evaluators", "[]");
        //SearchAlgorithm::add_succ_order_options(*this);
        SearchAlgorithm::add_options_to_feature(*this);
    }

    virtual shared_ptr<partially_symbolic::PartiallySymbolicSearch> create_component(const plugins::Options &options, const utils::Context &) const override {
        shared_ptr<partially_symbolic::PartiallySymbolicSearch> search_algorithm = make_shared<partially_symbolic::PartiallySymbolicSearch>(options);
        return search_algorithm;
    }
};

static plugins::FeaturePlugin<PSymbSearchFeature> _plugin;
}
