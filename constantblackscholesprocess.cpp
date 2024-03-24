
#include "constantblackscholesprocess.hpp"
#include <iostream>
#include <ql/processes/eulerdiscretization.hpp>



namespace QuantLib{

	ConstantBlackScholesProcess::ConstantBlackScholesProcess(double Underlying_Value_, double Rf_rate_, double Volatility_, double Dividend_):StochasticProcess1D(ext::make_shared<EulerDiscretization>())
	{
		Underlying_Value = Underlying_Value_;
		Rf_rate = Rf_rate_;
		Dividend = Dividend_;
		Volatility = Volatility_;		
	}
	Real ConstantBlackScholesProcess::x0() const {
		return Underlying_Value;
	}
	Real ConstantBlackScholesProcess::drift(Time t, Real x) const {
		return Rf_rate - Dividend - 0.5*Volatility*Volatility;
	}
	Real ConstantBlackScholesProcess::diffusion(Time t, Real x) const {
		return Volatility;
	}
	Real ConstantBlackScholesProcess::apply(Real x0, Real dx) const {
		return x0*std::exp(dx);
	}
}
