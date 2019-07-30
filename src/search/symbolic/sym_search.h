#ifndef SYMBOLIC_SYM_SEARCH_H
#define SYMBOLIC_SYM_SEARCH_H

#include "sym_estimate.h"
#include "sym_params_search.h"
#include "sym_state_space_manager.h"
#include "sym_utils.h"
#include <map>
#include <memory>
#include <vector>

namespace symbolic {
class SymController;

// We use this enumerate to know why the current operation was truncated
enum class TruncatedReason {
  FILTER_MUTEX,
  MERGE_BUCKET,
  MERGE_BUCKET_COST,
  IMAGE_ZERO,
  IMAGE_COST
};
std::ostream &operator<<(std::ostream &os, const TruncatedReason &dir);

class SymSearch {
protected:
  // Attributes that characterize the search:
  std::shared_ptr<SymStateSpaceManager>
      mgr; // Symbolic manager to perform bdd operations
  SymParamsSearch p;

  SymController *engine; // Access to the bound and notification of new
                         // solutions

public:
  SymSearch(SymController *eng, const SymParamsSearch &params);

  virtual ~SymSearch() {}

  SymStateSpaceManager *getStateSpace() { return mgr.get(); }

  std::shared_ptr<SymStateSpaceManager> getStateSpaceShared() const {
    return mgr;
  }

  bool isSearchable() const {
    return isSearchableWithNodes(p.getMaxStepNodes());
  }

  bool step() {
    return stepImage(p.getAllotedTime(nextStepTime()),
                     p.getAllotedNodes(nextStepNodesResult()));
  }

  virtual bool stepImage(int maxTime, int maxNodes) = 0;

  virtual int getF() const = 0;
  virtual bool finished() const = 0;

  virtual long nextStepTime() const = 0;
  virtual long nextStepNodes() const = 0;
  virtual long nextStepNodesResult() const = 0;

  virtual bool isSearchableWithNodes(int maxNodes) const = 0;
};
} // namespace symbolic
#endif // SYMBOLIC_SEARCH