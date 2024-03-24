
#include "constantblackscholesprocess.hpp"
#include <iostream>
#include <ql/processes/eulerdiscretization.hpp>


namespace QuantLib {

    ConstantBlackScholesProcess::ConstantBlackScholesProcess(double underlying_value_ , double risk_free_rate_, double volatility_ , double dividend_)
    :StochasticProcess1D(ext::make_shared<EulerDiscretization>())
       {underlying_value=underlying_value_;
        risk_free_rate=risk_free_rate_;
        volatility=volatility_;
        dividend=dividend_;}

    //Market value of the underlying
    Real ConstantBlackScholesProcess::x0() const {
        return underlying_value;
    }
    
    /*
    dlnS_t = (r-d-(sigma)^2/2)dt + sigma * dW_t
    the drift and diffusion methods are virtual in the base class.
    the drift method returns the one for the logarithm.
    the diffusion method returns the local volatility. 
    */
    Real ConstantBlackScholesProcess::drift(Time t, Real x) const {
        return risk_free_rate - dividend - 0.5*volatility*volatility;
    }

    Real ConstantBlackScholesProcess::diffusion(Time t, Real x) const {
        return volatility;
    }

    Real ConstantBlackScholesProcess::apply(Real x0, Real dx) const {
        return x0 * std::exp(dx);
    }

}