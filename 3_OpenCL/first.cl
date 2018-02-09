kernel
void
ArrayMult( global const float *dArray, global float *dSums)
{
    int gid = get_global_id( 0 );
    float sum=0.0;
    for( int shift = 0; shift < 32768; shift++ )
    {
         if((gid+shift) < 32678) 
         {
            sum+=dArray[shift]* dArray[gid+shift];
         }
         else 
         {
            continue;
         }
    }
    dSums[gid] = sum;
}
