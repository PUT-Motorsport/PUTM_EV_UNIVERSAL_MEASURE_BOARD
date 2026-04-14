#include "ADS114S08.h"
#include "gpio.h"
#include "spi.h"

namespace ADS114S08 {

ID id;
STATUS status;
INPMUX inpmux;
PGA pga;
DATARATE datarate;
REF ref;
IDACMAG idacmag;
IDACMUX idacmux;
VBIAS vbias;
SYS sys;
OFCAL0 ofcal0;
OFCAL1 ofcal1;
FSCAL0 fscal0;
FSCAL1 fscal1;
GPIODAT gpiodat;
GPIOCON gpiocon;

} // namespace ADS114S08
