
#include "constantblackscholesprocess.hpp"
#include <iostream>
#include <ql/processes/eulerdiscretization.hpp>


namespace QuantLib {

    ConstantBlackScholesProcess::ConstantBlackScholesProcess(double underlyingValue_, double riskFreeRate_, double volatility_, double dividend_)
        :StochasticProcess1D(ext::make_shared<EulerDiscretization>())
        {
        underlyingValue = underlyingValue_;
        riskFreeRate = riskFreeRate_;
        volatility = volatility_;
        dividend = dividend_;
        }

    Real ConstantBlackScholesProcess::x0() const {
        return getUnderlyingValue();
    }

    Real ConstantBlackScholesProcess::drift(Time t, Real x) const {
        return this->getRiskFreeRate() - this->getDividend() - 0.5 * getVolatility() * getVolatility();
    }

    Real ConstantBlackScholesProcess::diffusion(Time t, Real x) const {
        return this->getVolatility();
    }

    Real ConstantBlackScholesProcess::apply(Real x0, Real dx) const {
        return x0 * std::exp(dx);
    }
}
