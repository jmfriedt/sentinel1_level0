// because of the reconstruction process, we cannot save the sign in hcode as sign*mcode 
// since if the latter is 0 (which will lead to a non-0 value after reconstruction), the 
// sign is lost when storing -0. We must hence keep sign and magnitude separate as described 
// in the documentation, hence the use of this structure with sign and code magnitude
struct sh_code {int sign;int mcode;};

int packet_decode(unsigned char *,int,float*,float*,float*,float*,char*,int*); // FDBAQ
