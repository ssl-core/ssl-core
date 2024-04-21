# Setup
Some steps have to be performed in order to execute the script

# Downgrade matplotlib
The common_texification function was removed in Matplotlib versions 3.8.0 and later. So, you have to downgrade matplotlib. The version tested was matplotlib 3.7.1


# Add imports to your python file
import tikzplotlib
from matplotlib.lines import Line2D
from matplotlib.legend import Legend

# Workarounds of known tikzplotlib issue ('Line2D' object has no attribute '_us_dashSeq' )
https://github.com/nschloe/tikzplotlib/issues/580 <br>
https://github.com/nschloe/tikzplotlib/issues/567
## Fix 1


add the snippet bellow to your code

> `Line2D._us_dashSeq    = property(lambda self: self._dash_pattern[1])`<br>
> `Line2D._us_dashOffset = property(lambda self: self._dash_pattern[0])`<br>
> `Legend._ncol = property(lambda self: self._ncols)`

## Fix 2
find the location where the tikzplotlib was installed. You can run the following command
$ pip show tikzplotlib

go to:
/tikzplotlib/_path.py

replace:

> `dashSeq = line._us_dashSeq` </br> `dashOffset = line._us_dashOffset` 

with:

> `dashOffset, dashSeq = line._unscaled_dash_pattern`

you still have to add the code bellow to your file:
> `Legend._ncol = property(lambda self: self._ncols)`