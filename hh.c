

void  ToFixedChar (byte  *pos)
{
      byte  *mid;
      word hsize,start,end,index;

      if  (*pos<136||*pos>249) return;

      start=0;
      hsize=sizeof(jotable);
      end  =(hsize-1)/2-1;
      while (start<=end&&start<=hsize&&end<=hsize) {
         index=(start+end)/2;
         mid=jotable+index*2;
         if (*pos>*mid) start=index+1;
         else if (*pos<*mid) end=index-1;
         else if (*(pos+1)>*(mid+1))  start=index+1;
         else if (*(pos+1)<*(mid+1))  end=index-1;
         else {
/*
      *pos    =FixedGrfTable[0];
      *(pos+1)=FixedGrfTable[1];
              *pos    =(index/94)+0xB0;
              *(pos+1)=(index%94)+0xA1;
*/
              printf("s=%d m=%d e=%d %c%c\n",start,mid,end,*(pos) *(pos+1));
              return;
         }
      }
}

void main(int argc, char *argv[]) {
  byte temp[1300];
  int i;
  FILE *infile, *outfile;

}

void usage(char *filename) {
  printf("HANGUL johan -> KS code converter\n\n");
  printf("USAGE : %s infile outfile\n\n", filename);
}


  if(argc != 3) {
    usage(argv[0]);
    return;
  }

/*

  infile=fopen(argv[1], "r");
  outfile=fopen(argv[2], "w");
  if(infile==NULL || outfile==NULL) {
    printf("Can't open file.\n");
    printf("Please check file status.\n");
    printf("(File permission, etc...)\n");
    return;
  }
  
  memset(temp, 0x00, 1300);
  while(!feof(infile)) {
    fgets(temp, 1300, infile);
    for (i=0 ; i<strlen(temp); )
      if ((byte)temp[i]<128) i++;
      else  ToFixedChar(temp+i),i+=2;
    fputs(temp, outfile);
    memset(temp, 0x00, 1300);
  }

  fclose(infile);
  fclose(outfile);
*/
  printf("Successfully converted.\n");
