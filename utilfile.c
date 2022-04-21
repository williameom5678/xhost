unlink_file(const char *fmt,...)
{
	char buff[500];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	if (strlen(buff)>0) unlink(buff);
}

rm_dir(char *dir)
{
	char buff[500];
	if (strlen(dir)>0) {
		sprintf(buff,"rm -rf %s",dir);
		del_esc3(buff);
		system(buff);
	}
}

touch_file(const char *fmt,...)
{
	char buff[500];
	char buff2[500];
	struct stat stbuf;
	struct utimebuf timestamp,*times=&timestamp;
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	if (strlen(buff)<1) return;
	if (stat(buff,&stbuf)==-1) {
		create_file(buff);
		chmod(buff,0666);
		return;
	}
	time(&stbuf.st_atime);
	time(&stbuf.st_mtime);
	times->actime =stbuf.st_atime;
	times->modtime=stbuf.st_mtime;
	utime(buff,times);
	chmod(buff,0666);
}

create_file(char *filename)
{
	FILE *fp;
	if ((fp=fopen(filename,"w+"))!=NULL) fclose(fp);
}


mkdir3(char *buff)
{
	char buff2[200];
	if (strlen(buff)>0) {
		del_esc3(buff);
		sprintf(buff2,"mkdir -p %s",buff);system(buff2);
		sprintf(buff2,"chmod 777 %s",buff);system(buff2);
	}
}


// 현재 디렉토리의 화일의 수를 파악한다. ..,. 빼고
// 오류이면 -1;
int dir_filenum(char *dir)	  //화일의 수
{
   DIR *dp;
   struct dirent *fp;
   int count=0;

   if((dp= opendir(dir)) == NULL) return -1;
   /* 디렉토리 목록 얻기 */
	  /* 화일이름이 '.'나 '..'나 '.'로 시작되는 숨김화일이라면 그냥 통과*/
   while((fp= readdir(dp)) != NULL) if(*(fp->d_name) != '.') count++;
   closedir(dp);
   return (count);
}



// 현재 디렉토리의 화일증 1개를 return한다. ..,. 빼고
// 오류이면 -1;
dir_filename(char *buff,char *dir)
{
	DIR *dp;
	struct dirent *fp;
	int count=0;
	strcpy(buff,"");
	dp= opendir(dir);
	/* 디렉토리 목록 얻기 */
	/* 화일이름이 '.'나 '..'나 '.'로 시작되는 숨김화일이라면 그냥 통과*/
	while((fp= readdir(dp)) != NULL)
	   if(*(fp->d_name) != '.'&&strlen(fp->d_name)<=12)
			{
				strcpy(buff,fp->d_name);
				count++;
				break;
			}
	closedir(dp);
	return count;
}


file_size(char *filename)
{
	struct stat stbuf;
	if ((stat(filename,&stbuf))==-1) return 0;
	return stbuf.st_size;
}

count_line(char *filename)
{
	FILE *fp;
	int count=0;
	char c;
	if ((fp=fopen(filename,"r"))!=NULL) {
		while ((c=fgetc(fp))!=EOF) if (c=='\n') count++;
		fclose(fp);
	}
	return count;
}




file_copy(char *filename1,char *filename2)		 //1->2
{
	FILE *fp1,*fp2;
	int i,n;
	if (is_same(filename1,filename2)) {
		mess("%s 파일명동일",filename1);
		return No;
	}
	if (strlen(filename1)<1||strlen(filename2)<1) {
		mess("빈파일명 1:\"%s\" 2:\"%s\"",filename1,filename2);
		return No;
	}
	n=file_size(filename1);
	fp1=fopen(filename1,"r");
	fp2=fopen(filename2,"w");
	if (fp1!=NULL&&fp2!=NULL) {
		for (i=0;i<n;i++) fputc(fgetc(fp1),fp2);
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	chmod(filename2,0777);
	if (n!=file_size(filename2)) {
		mess("화일크기%d 오차%s(%d)->%s(%d)",n,filename1,file_size(filename1),filename2,file_size(filename2));
		return No;
	}
	return Yes;
}

//화일이 없으면 No 포함되면 Yes;
del_emptyline(char *filename)
{
	char buff[400];
	char buff2[400];
	FILE *fp,*fp2;
	char filename2[300];
	sprintf(filename2,"tmp/tmp3%d%s",cfg.hostno,no_spc(tty)); // 임시화일
	if ((fp=fopen(filename,"r"))!=NULL&&(fp2=fopen(filename2,"w+"))!=NULL) {
		while (fgets(buff,390,fp)!=NULL) {
			strcpy(buff2,buff);
			del_space(buff2);
			if (buff2[0]!=0) fprintf(fp2,"%s",buff);
		}
		fclose(fp);
		fclose(fp2);
		file_move(filename2,filename);
	}
}


//file1 에 file2를 추가함
file_add(char *filename1,char *filename2)
{
	FILE *fp1,*fp2;
	int i,n;
	if ((fp1=fopen(filename1,"a+"))==NULL) fp1=fopen(filename1,"w+");
	if (fp1!=NULL) {
		fp2=fopen(filename2,"r+");
		if (fp2!=NULL) {
			fseekend(fp2);
			n=ftell(fp2);
			rewind(fp2);
			for (i=0;i<n;i++) fputc(fgetc(fp2),fp1);
			fclose(fp2);
		}
		fclose(fp1);
	}
}


file_move(char *filename1,char *filename2)		 //1->2
{
	if (file_copy(filename1,filename2)) {
		unlink(filename1);
		return Yes;
	}
	return No;
}


time_t file_time(char *filename) {		 //file의 최종수정시간
	struct stat stbuf;
	if ((stat(filename,&stbuf))==-1) return 0;
	return (stbuf.st_mtime);
}

cfile_write(char *filename,char *id)
{
	FILE *fp;
	char tid[21];
	int last=0;
	xstrncpy(tid,id,12);
	if ((fp=fopen(filename,"r+"))==NULL) fp=fopen(filename,"w+");
	if (fp!=NULL) {
		fseekend(fp);
		last=ftell(fp)/sizeof(tid);
		fseek(fp,last*sizeof(tid),SEEK_SET);
		fwrite(&tid,sizeof(tid),1,fp);
		fclose(fp);
	}
}


cfile_count(char *filename,char *id,int max)
{
	FILE *fp;
	char tid[21];
	int count=0;
	if ((fp=fopen(filename,"r"))!=NULL) {
		while (fread(&tid,sizeof(tid),1,fp)) {
			if (is_same(tid,id)) count++;
			if (max>0&&count>=max) break;
		}
	}
	return count;
}



//화일속의 문자열 이 포함되면 Yes;
in_file(char *filename,char *buffstr)
{
	return in_file_(1,filename,buffstr);
}


//화일속의 문자열 이 포함되면 Yes; (동일하여야함)
in_file2(char *filename,char *buffstr)
{
	char buff[500];
	int found=No;
	FILE *fp;
	if ((fp=fopen(filename,"r"))!=NULL) {
		while (fgets(buff,200,fp)!=NULL) {
			del_enter(buff);
			if (is_same(buff,buffstr)) {
				found=Yes;
				break;
			}
		}
		fclose(fp);
	}
	return (found);
}


//문자열에 화일속의 내용이 포함되면 Yes;
in_file3(char *filename,char *buffstr)
{
	return in_file_(3,filename,buffstr);
}

//화일속의 문자열 이 포함되면 Yes; (30자까지만 비교 동일하여야함)
in_file4(char *filename,char *buffstr)
{
	return in_file_(2,filename,buffstr);
}

//빈칸으로 분리된
in_file5(char *filename,char *buffstr)
{
	FILE *fp;
	int i,key=No;
	char buff[300];
	if (strlen(buffstr)<1) return No;
	if ((fp=fopen(filename,"r"))==NULL) return No;
	while (fgets(buff,200,fp)!=NULL) {
		del_esc(buff);check_cmd(buff);
		key=No;
		for (i=0;i<10;i++) {
			if (is_same(mmc[i],buffstr)) {
				key=Yes;
				break;
			}
		}
	}
	fclose(fp);
	return key;
}


in_file_(int mode,char *filename,char *buffstr)
{
	char buff1[500];
	char buff2[500];
	int found=No;
	FILE *fp;
	strcpy(buff1,buffstr);
	if (mode==4) buff1[30]=0;
	del_clear(buff1);
	if (bit(allmode,11)) {
		printf("*INFILE=%s(%dB)*",filename,file_size(filename));
		getchar();
	}
	if ((fp=fopen(filename,"r+"))!=NULL) {
		while (fgets(buff2,200,fp)!=NULL) {
			del_clear(buff2);
			if (mode==4) buff2[30]=0;
			if ( strlen(buff2)>0&&(mode==1&&stristr(buff2,buff1)) ||
				 (mode==2&&strcasecmp(buff2,buff1)==0) ||
				 (mode==4&&strcasecmp(buff2,buff1)==0) ||
				 (mode==3&&stristr(buff1,buff2)) ) {
				found=Yes;
				break;
			}
		}
		fclose(fp);
	}
	return (found);
}



//화일명으로 적당한가?
good_filename(char *buff)
{
	int i=0,n=0;
	unsigned char c;
	if (buff[0]==0) return No;
	for (i=0;i<(int)strlen(buff);i++)
		if (buff[i]=='.') n++;
	if (n>1) return No;   //.이 1개이상일때
	while ((c=buff[i++])) {
			 if (c>'0'&&c<'9') ;
		else if (c>'a'&&c<'z') ;
		else if (c>'A'&&c<'Z') ;
		else if (c=='_'||c=='-'||c=='.') ;
		else {
			printf("\n %s 는 화일명으로 적합하지 못합니다.\n",buff);
			getchar();
			return No;
		}
	}
	return Yes;
}


//화일명을 서로 바꾼다.
file_change(char *filename1,char *filename2)
{
	char filename3[200];
	char buff[200];
		 if (!is_file(filename1)&&!is_file(filename2));
	else if (strcmp(filename1,filename2)==0);
	else if (!is_file(filename1)) rename(filename2,filename1);
	else if (!is_file(filename2)) rename(filename1,filename2);
	else {
		strcpy(filename3,filename1);
		strcat(filename3,"___");
		sprintf(buff,"cp %s %s -rf",filename1,filename3);del_esc3(buff);system(buff);
		sprintf(buff,"cp %s %s -rf",filename2,filename1);del_esc3(buff);system(buff);
		sprintf(buff,"cp %s %s -rf",filename3,filename2);del_esc3(buff);system(buff);
		sprintf(buff,"rm %s",filename3);del_esc3(buff);system(buff);
	}
}


//이 화일이 오래된 화일인가?
is_old_file(char *filename,int sec)
{
	time_t tnow,i;
	time(&tnow);   //현재시간
	i=file_time(filename);
	if ( (tnow-i)>sec ) return Yes;
	return No;
}

ch_dir(char *dirname)
{
	if (!is_dir(dirname)) {
		char olddir[200];
		getcwd(olddir,140); 	 //현재의 디렉토리
		mess("%s 디렉토리오류 (현재%s)",dirname,olddir);
		return No;
	}
	chdir(dirname);
	return Yes;
}

//현재의 디렉토리가 존재하는가? 있으면 Yes 없으면 No
is_dir(char *dirname)
{
	DIR *dp;
	struct dirent *fp;
	struct stat fs;
	if((dp= opendir(dirname)) == NULL) return No;
		/* 디렉토리 목록 얻기 */
	while((fp= readdir(dp)) != NULL) {
		if(strcmp(fp->d_name,".")==0) return Yes;      //.이 있다.
	}
	return No;
}

file_line(char *buff,char *filename,int line)
{
	char buff2[300];
	int n=1;
	int found=No;
	FILE *fp;
	strcpy(buff,"");
	if (line==0) line=1;
	if ((fp=fopen(filename,"r"))!=NULL) {
		strcpy(buff2,"");
		while (fgets(buff2,200,fp)!=NULL) {
			if (n++==line) {
				strcpy(buff,buff2);
				found=Yes;
				break;
			}
		}
		fclose(fp);
	}
	del_enter(buff);
	return (found);
}


file_line2(char *buff,char *filename,char *str)
{
	char buff1[300],buff2[300],buff3[300];
	int n=1;
	int found=No;
	FILE *fp;
	strcpy(buff,"");
	if ((fp=fopen(filename,"r"))!=NULL) {
		strcpy(buff2,"");
		while (fgets(buff2,200,fp)!=NULL) {
			sscanf(buff2,"%s%s",buff1,buff3);
			if (is_same(buff1,str)) {
				buff2cmd(buff2,buff,1);
				del_enter(buff);
				found=Yes;
				break;
			}
		}
		fclose(fp);
	}
	return (found);
}



file_line_num(char *buff,char *filename)
{
	char buff2[300];
	int n=1;
	int found=No;
	FILE *fp;
	strcpy(buff,"");
	if ((fp=fopen(filename,"r"))!=NULL) {
		strcpy(buff2,"");
		if (fgets(buff2,200,fp)) {
			found=Yes;
			del_enter(buff);
			strcpy(buff,buff2);
		}
		fclose(fp);
	}
	return (found);
}


put_line(char *buff,char *filename)
{
	FILE *fp;
	char buff2[3020];
	strcpy(buff2,buff);
	if ((fp=fopen(filename,"a+"))==NULL) fp=fopen(filename,"w+");
	if (fp!=NULL) {
		del_enter(buff2);
		fprintf(fp,"%s\n",buff2);
		fclose(fp);
	}
}


fget_str(char *buff,char *filename,int len)
{
	FILE *fp;
	strcpy(buff,"");
	if ((fp=fopen(filename,"r"))!=NULL) {
		fread(&buff[0],len,1,fp);
		fclose(fp);
		return Yes;
	}
	return No;
}

fput_str(char *buff,char *filename,int len)
{
	FILE *fp;
	if ((fp=fopen(filename,"w+"))!=NULL) {
		fwrite(&buff[0],len,1,fp);
		fclose(fp);
	}
}




buff2file(char *buff,char *filename)
{
	FILE *fp;
	if ((fp=fopen(filename,"w"))!=NULL) {
		fprintf(fp,"%s",buff);
		fclose(fp);
		chmod(filename,0777);
	}
}

is_samefile(char *filename1,char *filename2)
{
	FILE *fp1,*fp2;
	int same=Yes;

	fp1=fopen(filename1,"r+");
	fp2=fopen(filename2,"r+");
	if (fp1!=NULL&&fp2==NULL) same=No;
	if (fp1==NULL&&fp2!=NULL) same=No;
	if (same&&fp1!=NULL&&fp2!=NULL) {
		while (!feof(fp1)&&!feof(fp2)) {
			if (fgetc(fp1)!=fgetc(fp2)) {
				same=No;
				break;
			}
		}
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	return same;
}


file2buff(char *buff,char *filename,int max)
{
	FILE *fp;
	int i,len=0;
	char ch;
	if (max==0) max=99999999;
	strcpy(buff,"");
	if ((fp=fopen(filename,"r"))==NULL) return -1;
	fseekend(fp);
	len=ftell(fp);
	rewind(fp);
	i=0;
	while ((ch=fgetc(fp))!=EOF&&i<max&&i<=len) buff[i++]=ch;
	buff[i]=0;
	fclose(fp);
	return len;
}



file_dir(char *file,char *dir, char *filename)
{
	int n,i;
	char buff1[180];
	char buff2[180];

	strcpy(dir,"");
	strcpy(filename,"");
	bzero(buff1,150);
	bzero(buff2,150);
	strcpy(buff1,file);
	if (buff1[0]==0) return;
	for (i=(int)strlen(buff1)-2;i>=0;i--) {
		if (buff1[i]=='/') {
			strcpy(buff2,&buff1[i+1]);
			buff1[i+1]=0;
			break;
		}
	}
	if (i<0) {	  //없을경우
		strcpy(dir,"./");
		strcpy(filename,buff1);
	}
	else {
		strcpy(dir,buff1);
		strcpy(filename,buff2);
	}
}


count_rec(char *filename,int size)
{
	FILE *fp;
	int n=0;
	if (size>0&&(fp=fopen(filename,"r+"))!=NULL) {
		fseek(fp,1,SEEK_END);
		n=ftell(fp)/size;
		fclose(fp);
	}
	if (n<0) n=0;
	return n;
}


count_frec(FILE *fp,int size)
{
	int n1=ftell(fp),n2;
	fseek(fp,1,SEEK_END);
	n2=ftell(fp)/size;
	fseek(fp,n1,SEEK_SET);
	return n2;
}


pure_name(int mode,char *buff)			//화일명에서 순수한 화일부분만 뽑아냄
{
	char buff2[200];
	int i,i2=0;
	bzero(buff2,200);
	for (i=0;i<(int)strlen(buff);i++) {
		if		((mode==0||mode==2)&&buff[i]=='/') {i2=0;bzero(buff2,200);}
		else if (buff[i]==' '||buff[i]==0) break;
		else if (mode==2&&buff[i]=='.')    break;
		else if (i2>1&&buff2[i2]==' ')     break;
		else buff2[i2++]=buff[i];
	}
	del_space(buff2);
	del_esc(buff2);
	strcpy(buff,buff2);
}


//화일에서 저장번호읽어옴  mode:1:증가함
inc_numfile(int mode,char *filename)
{
	FILE *fp;
	char buff[100];
	int n=0,n2;
	c_lock(1,filename);
	if (mode==0) {
		if ((fp=fopen(filename,"r+"))!=NULL) {
			if (!fread(&n,sizeof(n),1,fp)) n=0;
			if (n<0) n=0;
			fclose(fp);
		}
	}
	else if (mode==1) {
		if ((fp=fopen(filename,"r+"))==NULL) fp=fopen(filename,"w+");
		if (fp!=NULL) {
			if (!fread(&n,sizeof(n),1,fp)) n=0;
			if (n<0) n=0;
			rewind(fp);
			n2=n+1;
			fwrite(&n2,sizeof(n),1,fp);
			fclose(fp);
		}
	}
	sync();fflush(NULL);
	c_lock(0,filename);
	return n;
}


//화일에 저장
save_numfile(int num,char *filename)
{
	FILE *fp;
	c_lock(1,filename);
	if ((fp=fopen(filename,"w+"))!=NULL) {
		fwrite(&num,sizeof(num),1,fp);
		fclose(fp);
	}
	c_lock(0,filename);
	sync();fflush(NULL);
	return num;
}


//화일에 저장
read_numfile(char *filename)
{
	FILE *fp;
	int num=0;
	if ((fp=fopen(filename,"r+"))!=NULL) {
		fread(&num,sizeof(num),1,fp);
		fclose(fp);
	}
	return num;
}



check_free()
{
	char buff[100];
	char buff1[50];
	char buff2[50];
	char buff3[50];
	char buff4[50];
	char TMP_FILE3[100];
	int ret=999999;

	sprintf(TMP_FILE3,"tmp/tmp3%d%s",cfg.hostno,no_spc(tty));   //호스트번호별임시화일
	while (1) {
		if (!is_file("bin/dfmain.sh")) break;
		sys_tem("bin/dfmain.sh > %s",TMP_FILE3);
		if (!file_line(buff,TMP_FILE3,2)) break;
		sscanf(buff,"%s%s%s%s",buff1,buff2,buff3,buff4);
		if (strlen(buff4)<1) break;
		ret=atoi(buff4)/1024;
		break;
	}
	unlink(TMP_FILE3);
	return ret;
}

how_old_file(char *filename)
{
	time_t now;
	struct stat stbuf;
	time(&now); //12311231
	if ((stat(filename,&stbuf))==-1) return now;
	return (now-stbuf.st_mtime);
}

