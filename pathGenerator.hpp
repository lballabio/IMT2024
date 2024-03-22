#ifndef pathGenerator_hpp
#define pathGenerator_hpp

#include <ql/exercise.hpp>
#include <ql/instruments/barrieroption.hpp>
#include <ql/pricingengines/mcsimulation.hpp>
#include <ql/pricingengines/barrier/mcbarrierengine.hpp>
#include <ql/processes/blackscholesprocess.hpp>
#include <utility>

namespace QuantLib {

    template <class RNG = PseudoRandom, class S = Statistics>
    class pathGeneratorConstruct{
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
                // Extracting the parameters here
                Time time = grid.back();
                double const_div = blackSC_process->dividendYield()->zeroRate(time, Continuous, NoFrequency);
                double const_rf = blackSC_process->riskFreeRate()->zeroRate(time, Continuous, NoFrequency);
                double const_volatility_ = blackSC_process->blackVolatility()->blackVol(time, strike);
                double spot = blackSC_process->x0();
    
                // Creating the constant BS Process
                ext::shared_ptr <constantblackscholesprocess> const_blackSC_process(new constantblackscholesprocess(spot, const_rf, const_div, const_volatility_));
                return ext::shared_ptr<path_generator_type>(
                    // Generator path with constant BS
                    new path_generator_type(const_blackSC_process, grid,generator,MCVanillaEngine<SingleVariate, RNG, S>::brownianBridge_));
                } else {
                    return ext::shared_ptr<path_generator_type>(
                                new path_generator_type(process_,
                                            grid, generator, brownianBridge_));
                }
            }        
        };
}
#endif
