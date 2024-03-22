#ifndef choose_path_generator_hpp
#define choose_path_generator_hpp

#include <ql/exercise.hpp>
#include <ql/instruments/barrieroption.hpp>
#include <ql/pricingengines/mcsimulation.hpp>
#include <ql/pricingengines/barrier/mcbarrierengine.hpp>
#include <ql/processes/blackscholesprocess.hpp>
#include <utility>

namespace QuantLib {

	template <class RNG = PseudoRandom, class S = Statistics>
	class builder {
		public:
		typedef
		typename McSimulation<SingleVariate,RNG,S>::path_generator_type 
			path_generator_type;
                
		auto getGenerator(TimeGrid timegrid,
						typename RNG::rsg_type gen,
						boost::shared_ptr<StochasticProcess> process_,
						bool brownianBridge_,
						double strike,
						bool constant) {

			// We differenciate between our constant parameter case and not constant
			if (constant) {
                ext::shared_ptr<GeneralizedBlackScholesProcess> BS_process = 
					ext::dynamic_pointer_cast<GeneralizedBlackScholesProcess>(process_);
				
				// Retrieve process parameters
				Time time = timegrid.back();
				double riskFreeRate_ = BS_process->riskFreeRate()->zeroRate(time, Continuous);
				double dividend_ = BS_process->dividendYield()->zeroRate(time, Continuous);
				double volatility_ = BS_process->blackVolatility()->blackVol(time, strike);
				double underlyingValue_ = BS_process->x0();
			
				//return the new generator created with above parameters and ConstantBlackScholesProcess
				ext::shared_ptr<ConstantBlackScholesProcess> Constant_BS_process(new ConstantBlackScholesProcess(underlyingValue_, riskFreeRate_, volatility_, dividend_));
				return ext::shared_ptr<path_generator_type>(
					new path_generator_type(Constant_BS_process, timegrid, gen, brownianBridge_)); 
			}
			else {
				return ext::shared_ptr<path_generator_type>(
					new path_generator_type(process_, timegrid, gen, brownianBridge_));
			}
		}
	};
}

#endif