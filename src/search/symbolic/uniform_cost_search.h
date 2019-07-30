#ifndef SYMBOLIC_UNIFORM_COST_SEARCH_H
#define SYMBOLIC_UNIFORM_COST_SEARCH_H

#include "closed_list.h"
#include "frontier.h"
#include "open_list.h"
#include "sym_bucket.h"
#include "sym_estimate.h"
#include "sym_state_space_manager.h"
#include "sym_utils.h"
#include "unidirectional_search.h"
#include <map>
#include <memory>
#include <vector>

namespace symbolic
{
/*
 * This class allows to perform a BDD search.  It is designed to
 * mantain the current state in the search.  We consider four
 * different points at which we may truncate the search:
 * pop(), filter_mutex(), expand_zero(), expand_cost()
 * We mantain 3 BDDs to know the current state: Stmp, S and Szero.
 * Briefly:
 * 1) if Sfilter, Szero and S are empty => pop() => Szero.
 * 2) else if Stfilter => filter_mutex() => Szero
 * 3) else if Szero => expand_zero => S (passing by Sfilter)
 * 4) else (S must have something) => expand_cost()
 *
 * Zero cost operators have been expanded iff !S.IsZero() && Szero.IsZero()
 */
class SymController;
class ClosedList;

class UniformCostSearch : public UnidirectionalSearch
{
  UnidirectionalSearch *parent; // Parent of the search

  // Current state of the search:
  OpenList open_list;
  Frontier frontier;

  SymStepCostEstimation estimationCost, estimationZero; // Time/nodes estimated
  // NOTE: This was used to estimate the time and nodes needed to
  // perform a step in case that the next bucket is still not prepared.
  // Now, we always prepare the next bucket and when that fails no
  // estimation is needed (the exploration is deemed as not searchable
  // and is worse than any other exploration which has its next bucket
  // to expand ready)
  // SymStepCostEstimation estimationDisjCost, estimationDisjZero;
  bool lastStepCost; // If the last step was a cost step (to know if we are in
                     // estimationDisjCost or Zero)

  int last_g_cost;

  SymExpStatistics stats;

  virtual bool initialization() const
  {
    return frontier.g() == 0 && lastStepCost;
  }

  /*
   * Check generated or closed states with other frontiers.  In the
   * original state space we obtain a solution (maybe suboptimal if
   * the states are not closed).
   */
  void checkCutOriginal(Bucket &bucket, int g);

  void closeStates(Bucket &bucket, int g);

  void prepareBucket();

  void computeEstimation(bool prepare);

  //////////////////////////////////////////////////////////////////////////////
public:
  UniformCostSearch(SymController *eng, const SymParamsSearch &params);
  UniformCostSearch(const UniformCostSearch &) = delete;
  UniformCostSearch(UniformCostSearch &&) = default;
  UniformCostSearch &operator=(const UniformCostSearch &) = delete;
  UniformCostSearch &operator=(UniformCostSearch &&) = default;
  virtual ~UniformCostSearch() = default;

  virtual bool finished() const
  {
    assert(!open_list.empty() || !frontier.empty() ||
           closed->getHNotClosed() == std::numeric_limits<int>::max());
    return open_list.empty() && frontier.empty();
  }

  virtual bool stepImage(int maxTime, int maxNodes);

  bool init(std::shared_ptr<SymStateSpaceManager> manager, bool fw,
            std::shared_ptr<ClosedList> closed_opposite =
                nullptr); // Init forward or backward search

  virtual bool isSearchableWithNodes(int maxNodes) const;

  virtual int getF() const override
  {
    return open_list.minNextG(frontier, mgr->getAbsoluteMinTransitionCost());
  }

  virtual int getG() const override
  {
    return frontier.empty() ? open_list.minG() : frontier.g();
  }

  BDD getClosedTotal();

  BDD notClosed();

  void desactivate();

  void filterDuplicates(Bucket &bucket);

  virtual long nextStepTime() const override;
  virtual long nextStepNodes() const override;
  virtual long nextStepNodesResult() const override;

  // Returns the nodes that have been expanded by the algorithm (closed without
  // the current frontier)
  BDD getExpanded() const;
  void getNotExpanded(Bucket &res) const;

  // void write(const std::string & file) const;

  void filterMutex(Bucket &bucket)
  {
    mgr->filterMutex(bucket, fw, initialization());
  }

private:
  void violated(TruncatedReason reason, double time, int maxTime, int maxNodes);

  friend std::ostream &operator<<(std::ostream &os,
                                  const UniformCostSearch &bdexp);
};

} // namespace symbolic
#endif