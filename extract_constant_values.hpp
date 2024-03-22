
#ifndef _EXTRACT_CONSTANT_VALUES_H
#define _EXTRACT_CONSTANT_VALUES_H

#include "constantblackscholesprocess.hpp"
#include <ql/pricingengines/vanilla/mcvanillaengine.hpp>
#include <ql/processes/blackscholesprocess.hpp>
#include <ql/termstructures/volatility/equityfx/blackconstantvol.hpp>
#include <ql/termstructures/volatility/equityfx/blackvariancecurve.hpp>
#include <boost/shared_ptr.hpp>
#include <ql/quantlib.hpp>
#include <iostream>

namespace QuantLib{


template <class ArgumentsType>

inline ext::shared_ptr<ConstantBlackScholesProcess> extract_constant_values(
    const boost::shared_ptr<GeneralizedBlackScholesProcess>& process,
    const ArgumentsType& arguments_)
{


    Time time_of_maturity = process->time(arguments_.exercise->lastDate());
    ext::shared_ptr<StrikedTypePayoff> payoff = 
        ext::dynamic_pointer_cast<StrikedTypePayoff>(arguments_.payoff);
    double strike = payoff->strike();
    double underlying_value_ = process->x0();
    double risk_free_rate_ = process->riskFreeRate()->zeroRate(time_of_maturity, Continuous);
    double dividend_ = process->dividendYield()->zeroRate(time_of_maturity, Continuous);
    double volatility_ = process->blackVolatility()->blackVol(time_of_maturity, strike);
    ext::shared_ptr<ConstantBlackScholesProcess> constantBlackScholesProcess(new ConstantBlackScholesProcess(underlying_value_,
                                                                                                            risk_free_rate_,
                                                                                                            volatility_,
                                                                                                            dividend_));

    return constantBlackScholesProcess;
    
}
    
}

#endif