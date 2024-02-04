
# Introduction to QuantLib - IMT 2024

## Coding project: use constant parameters in Monte Carlo engines

In Monte Carlo engines, repeated calls to the process methods may
cause a performance hit; especially when the process is an instance of
the `GeneralizedBlackScholesProcess` class, whose methods in
turn make expensive method calls to the contained term structures.

The performance of the engine can be increased at the expense of some
accuracy.  Create a new class that models a Black-Scholes process with
constant parameters (underlying value, risk-free rate, dividend yield,
and volatility); then modify the Monte Carlo engines copied in this
repository so that they still take a generic Black-Scholes process and
an additional boolean parameter.  If the boolean is `false`, the
engine runs as usual; if it is `true`, the engine extracts the
constant parameters from the original process (based on the exercise
date of the option; for instance, the constant risk-free rate should
be the zero-rate of the full risk-free curve at the exercise date) and
runs the Monte Carlo simulation with an instance of the constant
process.

**You should not modify the `main.cpp` file, only the engines and the
new constant process.**

After your modifications:

1. Compare the results (value, elapsed time) obtained with and without
   constant parameters.  The value with non-constant parameters should
   be the same returned from the original engine in QuantLib.  The
   value with constant parameters should be similar but not identical,
   and hopefully the elapsed time should decrease.

2. Try to avoid duplication between the three engines that you are
   modifying (European, Asian, barrier).  Ideally, some of the
   additional code (for instance, extracting the constant parameters
   based on exercise date and strike) should be written just once.

To compile and run the program on your machine you'll need QuantLib
installed; instructions to do that for different operating systems are
at <https://www.quantlib.org/install.shtml>.


## How to submit your solution

1. Get a GitHub account, if you don't have one already.
2. Clone this repository with the "Fork" button in the top right
   corner of the page (if you're not reading this on GitHub, go to
   <https://github.com/lballabio/IMT2024> first).
3. Check out your clone on your machine.
4. Modify the source files as required by the project.  Feel free to
   add any other file you might need. You can also add your report.
5. When you want me to see your progress (ideally, as soon as you have
   a sketch of the solution), push your changes to your clone and
   submit a pull request.  This is only needed the first time;
   afterwards, push the new changes to your fork and they will be
   added to the existing pull request automatically.

More detailed instructions for forking and creating pull requests are
available at <https://help.github.com/articles/fork-a-repo> and
<https://help.github.com/articles/using-pull-requests>.  A basic guide
to GitHub is at <https://guides.github.com/activities/hello-world/>.

Good luck!

