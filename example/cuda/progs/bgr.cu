#define NUMTHREADS 48
#define NUMBLOCKS 96

__global__ void FragmentProgram(int w, int h, unsigned char *in, unsigned char *out) {
  int i=0;

  for(i=threadIdx.x+blockIdx.x*ATHREAD_COUNT; i<parms.n; i+=blockDim.x*gridDim.x) {
    out[i*3]=in[i*3+2];
    out[i*3+1]=in[i*3+1];
    out[i*3+2]=in[i*3];
  }
}


