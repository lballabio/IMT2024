#ifndef extract_process_hpp
#define extract_process_hpp


#include <ql/exercise.hpp>
#include <ql/instruments/barrieroption.hpp>
#include <ql/timegrid.hpp>
#include <ql/processes/blackscholesprocess.hpp>
#include <ql/instruments/payoffs.hpp>
#include "constantblackscholesprocess.hpp"


namespace QuantLib{
	inline ext::shared_ptr<StochasticProcess> extract_process(
	bool additional_constParameters,
	TimeGrid grid,
	ext::shared_ptr<StochasticProcess> process,
	ext::shared_ptr<Payoff> payoff){

	    // boolean True
	    if (additional_constParameters){

	        ext::shared_ptr<GeneralizedBlackScholesProcess> BS_process = 
	        ext::dynamic_pointer_cast<GeneralizedBlackScholesProcess>(process);

	        // Parameters of the BS Process
	        Time time=grid.back();
	        double strike = ext::dynamic_pointer_cast<StrikedTypePayoff>(payoff) -> strike();
	        
	        double Rf_rate_ = BS_process -> riskFreeRate() -> zeroRate(time,Continuous);
	        double Dividend_ = BS_process -> dividendYield() -> zeroRate(time,Continuous);
	        double Volatility_ = BS_process -> blackVolatility() -> blackVol(time,strike);
	        double Underlying_Value_ = BS_process -> x0();

	        ext::shared_ptr<ConstantBlackScholesProcess> cst_BS_process(
	            new ConstantBlackScholesProcess(Underlying_Value_,Rf_rate_,Volatility_,Dividend_));

	        return cst_BS_process;
	    } 
	    // boolean False
	    else {
	        return process;
	    }
	}
}

#endif