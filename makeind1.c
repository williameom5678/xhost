#include "bbs.h"
#define MAX 10000
#define MAX2 50000
#define MAX3 100000
main(argc, argv)
int argc;
char *argv[];
{
    if (argc<3) sort_all(1);
    else if (atoi(argv[1])==1) make_sort(1,argv[2]);
    else if (atoi(argv[1])==2) make_sort(2,argv[2]);
    else if (atoi(argv[1])==3) make_sort(3,argv[2]);
    else                       make_sort(1,argv[1]);
}


sort_all(int mode)
{
    int i;
    char buff[100];
    for(i=40;i<210;i++) {
        sprintf(buff,"%c___",i);
        make_sort(mode,buff);
    }
}

make_sort(int mode,char *buff)
{
    if (strlen(buff)<1||strcasecmp(buff,"make_ind_all")==0) sort_all(mode);
    else if (mode==1) make_sort1(buff);
    else if (mode==2) make_sort2(buff);
    else if (mode==3) make_sort3(buff);
}


char *strlwr(byte *str)
{
	for(; *str ;str++)
	if(isupper(*str))
		*str=tolower(*str);
	return (str);
}



int comp_5(IND2 *ind1,IND2 *ind2) {
	return strcmp(ind1->id,ind2->id);
}

make_sort1(char *str)
{
	char buff[100];
    int i,n,count=0;
	PF pf2;
	FILE *fp;
	char ind_file[100];
	char ind2_file[100];
    IND ind;
    IND2 aind[MAX];

    strlwr(str);
    if (strlen(str)<2) return;
    sprintf(ind_file ,"id/id_%u.ind",(byte)str[0]);
	if ((fp=fopen(ID_FILE ,"r"))==NULL) return;
    sprintf(buff,"%c%c",(byte)255,(byte)255);
    for (i=0;i<MAX;i++) strcpy(aind[i].id,buff);
	n=0;
    count=0;
    while(count<MAX&&fread(&pf2,sizeof_pf,1,fp)) {
		strlwr(pf2.id);
        if (strlen(pf2.id)>=3&&(byte)pf2.id[0]==(byte)str[0]) {
            sprintf(aind[count].id,pf2.id);
            aind[count++].cnum=n;
		}
        n++;
	}
	fclose(fp);
    if (count<1) {unlink(ind_file);return;}
    if (count>(MAX-10)) printf("\n ### %s IND 파일 ID %d 초과\n",ind_file,MAX);
    qsort(aind,MAX,sizeof(aind[0]),comp_5);
	if ((fp=fopen(ind_file,"w"))!=NULL) {
        ind.ret='\n';
        for (i=0;i<count+1;i++) {
			if (strlen(aind[i].id)>2) {
//                printf("%d %s\n",i,aind[i].id);
                strcpy(ind.id,aind[i].id);
                sprintf(ind.cnum,"%u",aind[i].cnum);
                fwrite(&ind,sizeof_ind,1,fp);
			}
		}
		fclose(fp);
	}
}



make_sort2(char *str)
{
	char buff[100];
    int i,n,count=0;
	PF pf2;
	FILE *fp;
	char ind_file[100];
	char ind2_file[100];
    IND ind;
    IND2 aind[MAX2];

    strlwr(str);
    if (strlen(str)<2) return;
    sprintf(ind_file ,"id/id_%u.ind",(byte)str[0]);
	if ((fp=fopen(ID_FILE ,"r"))==NULL) return;
    sprintf(buff,"%c%c",(byte)255,(byte)255);
    for (i=0;i<MAX2;i++) strcpy(aind[i].id,buff);
	n=0;
    count=0;
    while(count<MAX2&&fread(&pf2,sizeof_pf,1,fp)) {
		strlwr(pf2.id);
        if (strlen(pf2.id)>=3&&(byte)pf2.id[0]==(byte)str[0]) {
            sprintf(aind[count].id,pf2.id);
            aind[count++].cnum=n;
		}
        n++;
	}
	fclose(fp);
    if (count<1) {unlink(ind_file);return;}
    if (count>(MAX2-10)) printf("\n ### %s IND 파일 ID %d 초과\n",ind_file,MAX2);
    qsort(aind,MAX2,sizeof(aind[0]),comp_5);
	if ((fp=fopen(ind_file,"w"))!=NULL) {
        ind.ret='\n';
        for (i=0;i<count+1;i++) {
			if (strlen(aind[i].id)>2) {
//                printf("%d %s\n",i,aind[i].id);
                strcpy(ind.id,aind[i].id);
                sprintf(ind.cnum,"%u",aind[i].cnum);
                fwrite(&ind,sizeof_ind,1,fp);
			}
		}
		fclose(fp);
	}
}


make_sort3(char *str)
{
	char buff[100];
    int i,n,count=0;
	PF pf2;
	FILE *fp;
	char ind_file[100];
	char ind2_file[100];
    IND ind;
    IND2 aind[MAX3];

    strlwr(str);
    if (strlen(str)<2) return;
    sprintf(ind_file ,"id/id_%u.ind",(byte)str[0]);
	if ((fp=fopen(ID_FILE ,"r"))==NULL) return;
    sprintf(buff,"%c%c",(byte)255,(byte)255);
    for (i=0;i<MAX3;i++) strcpy(aind[i].id,buff);
	n=0;
    count=0;
    while(count<MAX3&&fread(&pf2,sizeof_pf,1,fp)) {
		strlwr(pf2.id);
        if (strlen(pf2.id)>=3&&(byte)pf2.id[0]==(byte)str[0]) {
            sprintf(aind[count].id,pf2.id);
            aind[count++].cnum=n;
		}
        n++;
	}
	fclose(fp);
    if (count<1) {unlink(ind_file);return;}
    if (count>(MAX3-10)) printf("\n ### %s IND 파일 ID %d 초과\n",ind_file,MAX3);
    qsort(aind,MAX3,sizeof(aind[0]),comp_5);
	if ((fp=fopen(ind_file,"w"))!=NULL) {
        ind.ret='\n';
        for (i=0;i<count+1;i++) {
			if (strlen(aind[i].id)>2) {
//                printf("%d %s\n",i,aind[i].id);
                strcpy(ind.id,aind[i].id);
                sprintf(ind.cnum,"%u",aind[i].cnum);
                fwrite(&ind,sizeof_ind,1,fp);
			}
		}
		fclose(fp);
	}
}


