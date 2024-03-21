#ifndef _CONSTANTBLACKSCHOLESPROCESS_H
#define _CONSTANTBLACKSCHOLESPROCESS_H

#include <ql/stochasticprocess.hpp>

namespace QuantLib {

    class ConstantBlackScholesProcess : public StochasticProcess1D {

    public:
        ConstantBlackScholesProcess(double underlying_value_ , double risk_free_rate_, double volatility_ , double dividend_);
        

        Real x0() const override;
        Real drift(Time t, Real x) const override;
        Real diffusion(Time t, Real x) const override;
        Real apply(Real x0, Real dx) const override;

      private:
        double underlying_value;
        double risk_free_rate;
        double volatility;
        double dividend;
    };

}

#endif // _CONSTANTBLACKSCHOLESPROCESS_H