#ifndef quantlib_constant_black_scholes_process_hpp
#define quantlib_constant_black_scholes_process_hpp
#include <ql/stochasticprocess.hpp>

namespace QuantLib {

    class ConstantBlackScholesProcess : public StochasticProcess1D {

		public: 
			ConstantBlackScholesProcess(double underlyingValue_,
										double riskFreeRate_,
										double volatility_, 
										double dividend_);

			Real x0() const override;
			Real drift(Time t, Real x) const override;
			Real diffusion(Time t, Real x) const override;
			Real apply(Real x0, Real dx) const override;

		private:
			double underlyingValue;
			double riskFreeRate;
			double volatility;
			double dividend;
        
    };

}

#endif

