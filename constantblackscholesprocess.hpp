#ifndef CONSTANT_BLACK_SCHOLES_PROCESS_HPP
#define CONSTANT_BLACK_SCHOLES_PROCESS_HPP
#include <ql/stochasticprocess.hpp>

namespace QuantLib {

    class ConstantBlackScholesProcess : public StochasticProcess1D {

        public :
            ConstantBlackScholesProcess(double Underlying_Value_, double Rf_rate_, double Volatility_, double Dividend_);

            Real x0() const override;
            Real drift(Time t, Real x) const override;
            Real diffusion(Time t, Real x) const override;
            Real apply(Real x0, Real dx) const override;

        private :
            double Dividend;
            double Volatility;
            double Rf_rate;
            double Underlying_Value;
        
    };

}
#endif // CONSTANT_BLACK_SCHOLES_PROCESS_HPP
