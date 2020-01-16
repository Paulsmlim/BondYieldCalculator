# BondYieldCalculator

This program calculates:
1) The price of a bond given its coupon %, years, face value, and yield.
2) The yield of a bond given its coupon %, years, face value, and price.

The price of a bond is calculated using the annuity formula and adding the
discounted face value.

>Bond Price = C/r * ( 1 - 1/(1+r)^t ) + faceValue/(1+r)^t

The yield of a bond is calculated using a recursive algorithm similar to
the binary search algorithm. A lowYield and highYield is passed as arguments
to the recursive method, and using the average yield (midYield) of the
2 yields, a bond price is calculated using the other input parameters. If
the bond price calculated using this midYield is equal to the given bond
price entered by the user, the bond's yield is equal to midYield. This algorithm
is used with the assumption that the yield of a bond is in the interval [0.0, 1.0].

The program outputs the runtime of the calculation algorithm after it outputs
the bond's price or yield.
