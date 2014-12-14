//
//  main.m
//  C-serial
//
//  Created by Xiaoxian Duan on 12/7/14.
//  Copyright (c) 2014 XDuan. All rights reserved.
//

#import <Foundation/Foundation.h>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>


#define num_Asset  100  // number of Assets
#define num_Simu 10000 //number of Simus for every Asset
#define num_bin  50  ////number of bins

#define a 500 // random number generator range, or price range

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

int main(int argc, char *argv[])
{
    int tot_num = num_Asset * num_Simu;
    float price[tot_num]; //the complete array storing all the Simus for all Assets
    float price_base[num_Asset]; //the random number created as the base of the price of each Asset
    float price_Asset[num_Simu]; //the price array for one Asset;
    
    srand((unsigned int)time(NULL));
    
    for (int iAsset=0; iAsset<num_Asset; ++iAsset) //iAsset: price_base of i-th Asset
    {
      //  printf("Now generate the price of %d th Asset\n", iAsset+1);
        price_base[iAsset] = (float)rand()/(float)(RAND_MAX/a);
      //  printf("the price base of %d th Asset is %f\n", iAsset+1, price_base[iAsset]);
        
        for (int i_Simu=0; i_Simu<num_Simu; ++i_Simu) // //i_Simu: price Simus generated from price_base
        {
            price_Asset[i_Simu] = price_base[iAsset] * (float)rand()/(float)(RAND_MAX/1);
      //      printf("for the %d th Asset, the %d th price Simu is %f\n", iAsset,i_Simu, price_Asset[i_Simu]);
            
            price[ iAsset * num_Simu + i_Simu] = price_Asset[i_Simu];
            //   printf("for the %d th Asset, the %d th price Simu is %f\n", iAsset,i_Simu,price[ iAsset * num_Simu + i_Simu] );
        }
        
    }
    
    /*       for(int i =0; i<tot_num; i++) //test that all price Simus are stored into price[] array
     {
     printf("price Simu is %f\n", price[i] );
     }
     */
    //until now, we have generated the price Simus!
    
    //Below we start to calculate the mean, variance, and histogram
    
    float sum[num_Asset], mean[num_Asset], max_Asset[num_Asset], min_Asset[num_Asset], var[num_Asset], sdev[num_Asset]; //arrays storing sum, mean, max, min, variation of Assets
    
    float maximum = 0.0, minimum = 100000.0;
    
    for (int iAsset=0; iAsset<num_Asset; ++iAsset) //pbi: price_base of i-th Asset
    {
        sum[iAsset] = 0.0;
        
        for (int i_Simu=0; i_Simu< num_Simu; ++i_Simu) //pbi: price_base of i-th Asset
        {
            sum[iAsset] = sum[iAsset] + price[iAsset* num_Asset + i_Simu];
        }
        
     //   printf("sum of %d th Asset is %f\n", iAsset, sum[iAsset] );
        
        mean[iAsset] = sum[iAsset]/num_Simu;
        
        printf("price mean of %d th Asset is %f\n", iAsset, mean[iAsset] );
    }
    
    for (int iAsset=0; iAsset<num_Asset; ++iAsset) //pbi: price_base of i-th Asset
    {
        sdev[iAsset] = 0.0;
        
        for (int i_Simu=0; i_Simu< num_Simu; ++i_Simu) //pbi: price_base of i-th Asset
        {
            sdev[iAsset] +=(price[iAsset* num_Asset + i_Simu]- mean[iAsset])*(price[iAsset* num_Asset + i_Simu]- mean[iAsset])/(num_SIMU-1);
            
            
            
        }
        
        printf("price variation of %d th Asset is %f\n", iAsset, sdev[iAsset] );
        
    }
    
    
    
    for (int iAsset=0; iAsset < num_Asset; ++iAsset) //pbi: price_base of i-th Asset
    {
        
        max_Asset[iAsset]= maximum;
        min_Asset[iAsset] = minimum;
        
        
        for (int i = 0 + iAsset *num_Simu; i <  ( iAsset +1 ) *num_Simu; ++i) //pbi: price_base of i-th Asset
        {
            // printf("price array of is %f\n", price[iAsset * num_Asset + i_Simu+1]);
            max_Asset[iAsset]= MAX(max_Asset[iAsset],price[i]);
            min_Asset[iAsset]= MIN(min_Asset[iAsset],price[i]);
            
        }
        
        
    }
    
    
    for(int i = 0; i< num_Asset; ++i)
    {
        printf("price max of %d th Asset is %f\n", i, max_Asset[i] );
        printf("price min of %d th Asset is %f\n", i, min_Asset[i] );
    }
    
    
    
    float binsize[num_Asset];
    for (int iAsset=0; iAsset< num_Asset; ++iAsset ) //pbi: price_base of i-th Asset
    {
        binsize[iAsset] = (max_Asset[iAsset]-min_Asset[iAsset])/num_bin;
        printf("%d binsize %f\n",iAsset, binsize[iAsset]);
    }
    
    
    int bincount[num_bin];

  //  int iAsset =0;
  //  printf("iAsset %d\n", iAsset);
  //  printf("max  %d %f\n", iAsset, max_Asset[iAsset]);
  //  printf("binsize %d %f\n", iAsset, binsize[iAsset] );
    for (int iAsset = 0; iAsset< num_Asset; iAsset++)
    {
for(int i_bin= 0; i_bin< num_bin; ++i_bin)
{
    bincount[i_bin] =0;
    for(int i_Simu= 0;i_Simu< num_Simu;++i_Simu)
    {
       // printf("i_Simu %d\n",i_Simu);
       // printf("price %f\n",price[i_Simu]);
        
        if( price[i_Simu+iAsset*num_Simu]>= min_Asset[iAsset] + i_bin * binsize[iAsset] && price[i_Simu+iAsset*num_Simu] <= min_Asset[iAsset] + (i_bin+1) *binsize[iAsset])
        {
         
            bincount[i_bin] = bincount[i_bin]+1;
         }
    }
    if (i_bin==num_bin -1){bincount[i_bin]=bincount[i_bin]+1;}
}
if (i_bin=num_bin -1)
        
for(int j=0;j<num_bin;j++)
{
    printf("bincount %d is %d\n",j, bincount[j]);
}
}
}
