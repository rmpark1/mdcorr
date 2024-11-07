#include <cmath>

int main(int argc, char *argv[]) {
    return 0;
    // // fftw_complex *in, *out;
    // double *in;
    // fftw_complex *out;
    // fftw_plan p;
    //
    // int N = 2000;
    //
    // // in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    // in = (double*) malloc(sizeof(double)*N);
    // out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    //
    // p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
    //
    // // pb = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    //
    // for (int i = 0; i < N; i++) {
    //     // in[i][0] = cos(4*double(i)/N);
    //     // in[i][1] = 0.;
    //     in[i] = cos(4*double(i)/N);
    // }
    //
    // fftw_execute(p);
    //
    // // fftw_execute(pb);
    //
    // for (int i = 0; i < N; i++) {
    //     out[i][0] = out[i][0]/N;
    //     out[i][1] = out[i][1]/N;
    // }
    // for (int i = 0; i < N; i++) {
    //     printf("%e ", out[i][0]);
    //     printf("%e\n", out[i][1]);
    // }
    //
    // fftw_destroy_plan(p);
    // fftw_free(in); fftw_free(out);
}
