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
            ext::shared_ptr <GeneralizedBlackScholesProcess> blackSC_process =
                ext::dynamic_pointer_cast<GeneralizedBlackScholesProcess>(this->process_);
            Time time = grid.back();
            double const_div = blackSC_process->dividendYield()->zeroRate(time, Continuous, NoFrequency);
            double const_rf = blackSC_process->riskFreeRate()->zeroRate(time, Continuous, NoFrequency);
            double const_volatility_ = blackSC_process->blackVolatility()->blackVol(time, strike);
            double spot = blackSC_process->x0();

            //constantblackscholesprocess* const_blackSC_process = new constantblackscholesprocess(spot, const_rf, const_div, const_volatility_);
            ext::shared_ptr <constantblackscholesprocess> const_blackSC_process(new constantblackscholesprocess(spot, const_rf, const_div, const_volatility_));
            return ext::shared_ptr<path_generator_type>(
                new path_generator_type(const_blackSC_process, grid,generator,MCVanillaEngine<SingleVariate, RNG, S>::brownianBridge_));
            } else {
                //ext::shared_ptr<GeneralizedBlackScholesProcess> process_;  
                return ext::shared_ptr<path_generator_type>(
                        new path_generator_type(MCVanillaEngine<SingleVariate, RNG, S>::process_, grid,
                                                generator, MCVanillaEngine<SingleVariate, RNG, S>::brownianBridge_));
            }
        }        
    };
}

