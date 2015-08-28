#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <string.h>
#include "utils.h"

char chr(int i) {
    if(i<0) i = -i;
    return (char)i;
}

int ord(char *c) {
    int i = (int)*c;
    if(i<0) return 256 + i;
    return i;
}

char* pr_encrypt(char* in, char* key) {

    size_t in_l = strlen(in);
    char *cc, *ck, *cr;

    cr = malloc(sizeof(char)*(in_l));

    int i = 0;
    for(i; i<in_l; i++) {
        cc = substr(in, i, 1, NULL);
        ck = substr(key, (i % strlen(key)) - 1, 1 , NULL);
        cr[i] = chr(ord(cc) + ord(ck));
    };
    cr[i] = '\0';
    
    size_t cr_l = strlen(cr);
    int x = cr_l % 3 > 0 ? cr_l / 3 + 1 : cr_l / 3;
    size_t out_l = x * 4;
    char* out = malloc(out_l+1);

    base64encode(cr, cr_l, out, out_l);
    out[out_l] = '\0';
    
    return out;
}

char* pr_decrypt(char* in, char* key) {

    size_t in_l = strlen(in);
    size_t out_l = in_l * 3 / 4 + 10;
    unsigned char *out = (unsigned char *)malloc(out_l);

    int i = 0;
    char *cc, *ck, *cr;
    
    base64decode(in, in_l, out, &out_l);

    cr = malloc(sizeof(char)*(out_l));

    for(i; i<out_l; i++) {
        cc = substr(out, i, 1, NULL);
        ck = substr(key, (i % strlen(key)) - 1, 1 , NULL);
        cr[i] = chr(ord(cc) - ord(ck));
    };
    cr[i] = '\0';
    
    return cr;
}

char* substr (const char* string, int pos, int len, const char* replace) {
    char* substring;
    int   i;
    int   length;
 
    if (string == NULL)
        return NULL;
    length = strlen(string);
    if (pos < 0) {
        pos = length + pos;
        if (pos < 0) pos = 0;
    }
    else if (pos > length) pos = length;
    if (len <= 0) {
        len = length - pos + len;
        if (len < 0) len = length - pos;
    }
    if (pos + len > length) len = length - pos;
    if (replace != NULL) {
        if ((substring = malloc(sizeof(*substring)*(length-len+strlen(replace)+1))) == NULL)
            return NULL;
        for (i = 0; i != pos; i++) substring[i] = string[i];
        pos = pos + len;
        for (len = 0; replace[len]; i++, len++) substring[i] = replace[len];
        for (; string[pos]; pos++, i++) substring[i] = string[pos];
        substring[i] = '\0';
    }
    else {
        if ((substring = malloc(sizeof(*substring)*(len+1))) == NULL)
            return NULL;
        len += pos;
        for (i = 0; pos != len; i++, pos++)
            substring[i] = string[pos];
        substring[i] = '\0';
    }
 
    return substring;
}

int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize) {
   const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   const uint8_t *data = (const uint8_t *)data_buf;
   size_t resultIndex = 0;
   size_t x;
   uint32_t n = 0;
   int padCount = dataLength % 3;
   uint8_t n0, n1, n2, n3;

   /* increment over the length of the string, three characters at a time */
   for (x = 0; x < dataLength; x += 3) 
   {
      /* these three 8-bit (ASCII) characters become one 24-bit number */
      n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((x+1) < dataLength)
         n += ((uint32_t)data[x+1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((x+2) < dataLength)
         n += data[x+2];

      /* this 24-bit number gets separated into four 6-bit numbers */
      n0 = (uint8_t)(n >> 18) & 63;
      n1 = (uint8_t)(n >> 12) & 63;
      n2 = (uint8_t)(n >> 6) & 63;
      n3 = (uint8_t)n & 63;
            
      /*
       * if we have one byte available, then its encoding is spread
       * out over two characters
       */
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n0];
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n1];

      /*
       * if we have only two bytes available, then their encoding is
       * spread out over three chars
       */
      if((x+1) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n2];
      }

      /*
       * if we have all three bytes available, then their encoding is spread
       * out over four characters
       */
      if((x+2) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n3];
      }
   }  

   /*
    * create and add padding that is required if we did not have a multiple of 3
    * number of characters available
    */
   if (padCount > 0) 
   { 
      for (; padCount < 3; padCount++) 
      { 
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = '=';
      } 
   }
   if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
   result[resultIndex] = 0;
   return 0;   /* indicate success */
}

int base64decode (char *in, size_t inLen, unsigned char *out, size_t *outLen) { 
    char *end = in + inLen;
    char iter = 0;
    size_t buf = 0, len = 0;
    
    while (in < end) {
        unsigned char c = d[*in++];
        
        switch (c) {
        case WHITESPACE: continue;   /* skip whitespace */
        case INVALID:    return 1;   /* invalid input, return error */
        case EQUALS:                 /* pad character, end of data */
            in = end;
            continue;
        default:
            buf = buf << 6 | c;
            iter++; // increment the number of iteration
            /* If the buffer is full, split it into bytes */
            if (iter == 4) {
                if ((len += 3) > *outLen) return 1; /* buffer overflow */
                *(out++) = (buf >> 16) & 255;
                *(out++) = (buf >> 8) & 255;
                *(out++) = buf & 255;
                buf = 0; iter = 0;

            }   
        }
    }
   
    if (iter == 3) {
        if ((len += 2) > *outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 10) & 255;
        *(out++) = (buf >> 2) & 255;
    }
    else if (iter == 2) {
        if (++len > *outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 4) & 255;
    }

    *outLen = len; /* modify to reflect the actual output size */
    return 0;
}
