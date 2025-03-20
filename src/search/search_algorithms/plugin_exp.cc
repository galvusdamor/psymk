#include "explicit_search.h"
#include "search_common.h"

#include "../plugins/plugin.h"

using namespace std;

namespace plugin_exp {
class ExpSearchFeature : public plugins::TypedFeature<SearchAlgorithm, explicit_search::ExplicitSearch> {
public:
    ExpSearchFeature() : TypedFeature("exp") {
        document_title("Explicit Search");
        document_synopsis("");
        
		add_option<shared_ptr<Evaluator>>("eval", "evaluator for h-value");
        add_option<bool>("reopen_closed", "reopen closed nodes", "false");

        //add_option<shared_ptr<OpenListFactory>>("open", "open list");
        //add_list_option<shared_ptr<Evaluator>>(
        //    "preferred",
        //    "use preferred operators of these evaluators", "[]");
        //SearchAlgorithm::add_succ_order_options(*this);
        SearchAlgorithm::add_options_to_feature(*this);
        cout << "IN CONF" << endl;
    }

    virtual shared_ptr<explicit_search::ExplicitSearch> create_component(const plugins::Options &options, const utils::Context &) const override {
         cerr << "Hello!" <<endl;
        shared_ptr<explicit_search::ExplicitSearch> search_algorithm = make_shared<explicit_search::ExplicitSearch>(options);
        return search_algorithm;
    }
};

static plugins::FeaturePlugin<ExpSearchFeature> _plugin;
}
