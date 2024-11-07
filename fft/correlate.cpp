#include <cmath>
// #include <fftw3.h>

namespace fft {

void autocorrelate(double ***in) {
    // Qt = 1/(x.size/N * (N-np.arange(N)))
    // other = tuple(a for a in range(x.ndim) if a != axis % x.ndim)
    // conv = fftconvolve(x, np.flip(x, axis=axis), axes=axis).sum(axis=other)
    // return Qt*conv[-N:]
}

}
