#ifndef pathGenerator_hpp
#define pathGenerator_hpp

#include <ql/exercise.hpp>
#include <ql/instruments/barrieroption.hpp>
#include <ql/pricingengines/mcsimulation.hpp>
#include <ql/pricingengines/barrier/mcbarrierengine.hpp>
#include <ql/processes/blackscholesprocess.hpp>
#include <utility>

namespace QuantLib {

    // Class template for generating path generators
    template <class RNG = PseudoRandom, class S = Statistics>
    class pathGeneratorConstruct {
    public:
        
        // Define the path generator type based on McSimulation
        typedef typename McSimulation<SingleVariate, RNG, S>::path_generator_type path_generator_type;

        // Method for getting a path generator
        auto getPathGenerator(TimeGrid grid,
                               typename RNG::rsg_type generator,
                               boost::shared_ptr<StochasticProcess> process_,
                               bool brownianBridge_,
                               double strike,
                               bool _constantParameters) {
            // Here we distinguish between constant and non-constant parameter cases
            if (_constantParameters) {
                // If parameters are constant, construct a constant Black-Scholes process
                ext::shared_ptr<GeneralizedBlackScholesProcess> blackSC_process =
                    ext::dynamic_pointer_cast<GeneralizedBlackScholesProcess>(process_);
                // This is where we write the code to extract parameters once and for all
                Time time = grid.back();
                double const_div = blackSC_process->dividendYield()->zeroRate(time, Continuous, NoFrequency);
                double const_rf = blackSC_process->riskFreeRate()->zeroRate(time, Continuous, NoFrequency);
                double const_volatility_ = blackSC_process->blackVolatility()->blackVol(time, strike);
                double spot = blackSC_process->x0();

                // Creating the constant Black-Scholes Process
                ext::shared_ptr<constantblackscholesprocess> const_blackSC_process(new constantblackscholesprocess(spot, const_rf, const_div, const_volatility_));
                return ext::shared_ptr<path_generator_type>(
                    // Generate path with constant Black-Scholes process
                    new path_generator_type(const_blackSC_process, grid, generator, brownianBridge_));
            } else {
                // If parameters are not constant, directly generate path
                return ext::shared_ptr<path_generator_type>(
                    new path_generator_type(process_, grid, generator, brownianBridge_));
            }
        }        
    };
}

#endif

