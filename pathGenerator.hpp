#ifndef pathGenerator_hpp
#define patheGenerator_hpp

#include <ql/exercise.hpp>
#include <ql/instruments/barrieroption.hpp>
#include <ql/pricingengines/mcsimulation.hpp>
#include <ql/pricingengines/barrier/mcbarrierengine.hpp>
#include <ql/processes/blackscholesprocess.hpp>
#include <utility>

namespace QuantLib {

    template <class RNG = PseudoRandom, class S = Statistics>
    class pathGenerator{
      public:

      typedef
      typename McSimulation<SingleVariate, RNG,S>::path_generator_type
          path_generator_type;

    auto getPathGenerator(TimeGrid grid,
                          typename RNG::rsg_type generator,
                          boost::shared_ptr<StochasticProcess> process_,
                          bool brownianBridge_,
                          double strike,
                          bool _constantParameters) {
      // Here we distinguish both cases. At first, constant case
      if (_constantParameters) {
          ext::shared_ptr<GeneralizedBlackScholesProcess> BS_process =
              ext::dynamic_pointer_cast<GeneralizedBlackScholesProcess>(process_);
          Time extract_time = grid.back();
        
    }
}
