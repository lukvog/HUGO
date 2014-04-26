#ifndef HUGO_TRANSDUCER_H_
#define HUGO_TRANSDUCER_H_

// Author: Sławek Staworko <staworko@gmail.com>

struct Signature
{
   int  dimention;
   int* bounds;
              
   Signature(int);
};

struct Configuration
{
   Configuration(char* config_file, Signature* sig);
};



struct Transducer
{
   Transducer(Configuration* config);
   ~Transducer();

   int* consume(int in[3], int* output_buf);
};

#endif // HUGO_TRANSDUCER_H_

