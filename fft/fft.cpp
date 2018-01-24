
/***************************************************************************************
 *
 * Copyright (c) 2017 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: fft.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 11/25/2017 06:07:32 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/

#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
typedef struct {
  double re;
  double im;
}Compsition;

void swap(double *a, double *b) {
  double tmp = *a;
  *a = *b;
  *b = tmp;
}

void four1(double* data, unsigned long nn)
{
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;
 
    // reverse-binary reindexing
    n = nn<<1;
    j=1;
    for (i=1; i<n; i+=2) {
        if (j>i) {
            swap(&data[j-1], &data[i-1]);
            swap(&data[j], &data[i]);
        }
        m = nn;
        while (m>=2 && j>m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    };

    // here begins the Danielson-Lanczos section
    mmax=2;
    while (n>mmax) {
        istep = mmax<<1;
        theta = -(2*M_PI/mmax);
        wtemp = sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m=1; m < mmax; m += 2) {
            for (i=m; i <= n; i += istep) {
                j=i+mmax;
                tempr = wr*data[j-1] - wi*data[j];
                tempi = wr * data[j] + wi*data[j-1];
 
                data[j-1] = data[i-1] - tempr;
                data[j] = data[i] - tempi;
                data[i-1] += tempr;
                data[i] += tempi;
            }
            wtemp=wr;
            wr += wr*wpr - wi*wpi;
            wi += wi*wpr + wtemp*wpi;
        }
        mmax=istep;
    }
}
 
void fast_fft(double *x, double *re, double *im, int n) {
  int w = 1;
  while(w < n) {
    
    w = w << 1;
  }
}

//实数fft
void fft(double *x, double *re, double *im, double *tmp, int n) {
    if(n == 1) {
      re[0] = x[0];
      im[0] = 0.0;
      return;
    }

    int l = n / 2;
    for(int i = 0; i < l; ++i) {
        tmp[i] = x[2 * i];
        tmp[i + l] = x[2 * i + 1];
    }
    
    fft(tmp, re, im, x, l);
    fft(tmp + l, re + l, im + l, x + l, l);
    
    for(int i = 0; i < l; ++i) {
      double a = re[i];
      double b = im[i];
      double c = re[i + l];
      double d = im[i + l];
      double e = cos(-2.0 * i * M_PI / n);
      double f = sin(-2.0 * i * M_PI / n);
      re[i] = a + (e * c - d * f);
      im[i] = b + (c * f + d * e);
      re[i + l] = a + (d * f - e * c);
      im[i + l] = b - (c * f + d * e);
    }
}

//复数fft
void fft(Compsition *x, double *re, double *im, Compsition *tmp, int n) {
    if(n == 1) {
      re[0] = x[0].re;
      im[0] = x[0].im;
      return;
    }

    int l = n / 2;
    for(int i = 0; i < l; ++i) {
        tmp[i] = x[2 * i];
        tmp[i + l] = x[2 * i + 1];
    }
    
    fft(tmp, re, im, x, l);
    fft(tmp + l, re + l, im + l, x + l, l);
    
    for(int i = 0; i < l; ++i) {
      double a = re[i];
      double b = im[i];
      double c = re[i + l];
      double d = im[i + l];
      double e = cos(-2.0 * i * M_PI / n);
      double f = sin(-2.0 * i * M_PI / n);
      double e_c = e * c;
      double d_f = d * f;
      double c_f = c * f;
      double d_e = d * e;
      re[i] = a + (e_c - d_f);
      im[i] = b + (c_f + d_e);
      re[i + l] = a + (d_f - e_c);
      im[i + l] = b - (c_f + d_e);
    }
}

void fast_idft(double *data, int n) {
  for(int i = 0; i < n; ++i) data[2 * i + 1] = -data[2 * i + 1];
  four1(data, n);
  for(int i = 0; i < n; ++i) {
    data[2 * i] /= n;
    data[2 * i + 1] = -(data[2 * i + 1]) / n;
  }
}

//idft
void idft(Compsition *x, double *re, double *im, Compsition *tmp, int n) {
  for(int i = 0; i < n; ++i) x[i].im = -x[i].im;
  fft(x, re, im, tmp, n);
  for(int i = 0; i < n; ++i) {
    x[i].re = re[i] / n;
    x[i].im = -im[i] / n;
  }
}

void slow_convolution(double *h, double *x, int lh, int lx, double *result) {
  for(int i = 0; i < lx; ++i) {
    double tmp = 0.0;
    for(int j = 0; j < i && j < lh; ++j) {
      tmp += h[j] * x[i - j];
    }
    result[i] = tmp;
  }
}

void fast_convolution(double *h, double *x, int lh, int lx, double *result) {
  int l = 2;
  while(l < lx + lh) l = l << 1;
  double *xx = new double[l << 1];
  double *hh = new double[l << 1];
  memset(xx, 0x00, sizeof(double) * (l << 1));
  memset(hh, 0x00, sizeof(double) * (l << 1));
  for(int i = 0; i < lx; ++i) xx[2 * i] = x[i];
  for(int i = 0; i < lh; ++i) hh[2 * i] = h[i];
  four1(xx, l);
  four1(hh, l);
  for(int i = 0; i < l; ++i) {
    double a = xx[2 * i];
    double b = xx[2 * i + 1];
    double c = hh[2 * i];
    double d = hh[2 * i + 1];
    xx[2 * i] = a * c - b * d;
    xx[2 * i + 1] = a * d + b * c;
  }
  fast_idft(xx, l);
  for(int i = 0; i < lx; ++i) {
    result[i] = xx[2 * i];
  }

  delete [] xx;
  delete [] hh;
}

//一维卷积
void convolution(double *h, double *x, int lh, int lx, double *result) {
  int l = 2;
  while(l < lx + lh) l = l * 2;
  double *xx = new double[l];
  double *hh = new double[l];
  double *tmp = new double[l];
  double *re1 = new double[l];
  double *im1 = new double[l];
  double *re2 = new double[l];
  double *im2 = new double[l];
  Compsition *comp = new Compsition[l];
  Compsition *tmp_comp = new Compsition[l];
  for(int i = 0; i < lx; ++i) xx[i] = x[i];
  for(int i = 0; i < lh; ++i) hh[i] = h[i];
  for(int i = lx; i < l; ++i) xx[i] = 0.0;
  for(int i = lh; i < l; ++i) hh[i] = 0.0;
  fft(xx, re1, im1, tmp, l);
  fft(hh, re2, im2, tmp, l);

  for(int i = 0; i < l; ++i) {
    double a = re1[i];
    double b = im1[i];
    double c = re2[i];
    double d = im2[i];
    comp[i].re = a * c - b * d;
    comp[i].im = a * d + b * c;
  }

  idft(comp, re1, im1, tmp_comp, l);
  for(int i = 0; i < lx; ++i) {
    result[i] = comp[i].re;
  }

  delete [] xx;
  delete [] hh;
  delete [] tmp;
  delete [] re1;
  delete [] im1;
  delete [] re2;
  delete [] im2;
  delete [] comp;
}

int main() {
    
    double data1[16] = {2, 0, 3, 0,  3, 0, 2, 0};
    four1(data1, 4);

    /*  
    double data[4] = {2.0, 3.0, 3.0, 2.0};  
    double tmp_data[4], im[4], re[4], tmp_im[4], tmp_re[4];
    Compsition x[4], tmp_x[4];
    x[0].re = 10; x[0].im = 0;
    x[1].re = -1; x[1].im = -1;
    x[2].re = 0;  x[2].im = 0;
    x[3].re = -1; x[3].im = 1;
    fft(data, re, im, tmp_data, 4);
    for(int i = 0; i < 4; ++i) printf("%.2lf %.2lf\n", re[i], im[i]);
    idft(x, re, im, tmp_x, 4);
    for(int i = 0; i < 4; ++i) printf("%.2lf %.2lf\n", x[i].re, x[i].im);
    
    double h[4] = {1.0, 2.0, 3.0, 4.0};
    double y[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    double result[8];
    */
     
    freopen("data.in", "r", stdin);
    int n, m;
    double x[50000], h[50000], result[50000];
    struct timeval b, e;
    int tot = 0;
    while(scanf("%d %d", &n, &m) != EOF) {
      int xx, hh;
      for(int i = 0; i < n; ++i) scanf("%lf", &x[i]);
      for(int i = 0; i < m; ++i) scanf("%lf", &h[i]);

      gettimeofday(&b, NULL);
      fast_convolution(h, x, m, n, result);
      gettimeofday(&e, NULL);
      int cost = (e.tv_sec - b.tv_sec) * 1000 + (e.tv_usec - b.tv_usec) / 1000;
      tot += cost;
    }

    printf("%dms\n", tot);
    //for(int i = 0; i < 8; ++i) printf("%.2lf ", result[i]);
    //printf("\n");
    return 0;    
} 

