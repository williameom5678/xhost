

    file_add(buff
    fp1=fopen(TMP_FILE2,"w");
	fp2=fopen(TMP_FILE,"r");
    if (fp1==NULL||fp2==NULL) return No;
	if (mode==0) fprintf(fp1,"\n   ###### [답변");
    else         fprintf(fp1,"추가");


	unlink(TMP_FILE1);
	unlink(TMP_FILE2);
	unlink(TMP_FILE3);

	oldbbs=bbs;

	//내용을 임시화일로 저장  ->TMP_FILE3
	if (!is_file(NOW_FILED)) touch_file(NOW_FILED);
	fp1=fopen(NOW_FILED,"r");
	fp2=fopen(TMP_FILE3,"w");
	fseek(fp1,bbs.posi,SEEK_SET);
	for (i=0;i<bbs.size;i++) fputc(fgetc(fp1),fp2);
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);


	//3. 새로 추가된 내용(TMP_FILE)을 원래의 내용(TMP_FILE3)에 추가
	fp1=fopen(TMP_FILE3,"a+");
	fp2=fopen(TMP_FILE,"r");
	if (mode==0) fprintf(fp1,"\n   ###### [답변");
	else		 fprintf(fp1,"\n   ###### [추가");
	fprintf(fp1,"%d] ID: %s (%s) 일시 %s ######\n",calc_ok(3)+1,pf.id,pf.nick,now_str(3));

	fseekend(fp2);
	n=ftell(fp2) ;				//새로작성한 게시물의 길이
	rewind(fp2);
	for (i=0;i<n;i++) fputc(fgetc(fp2),fp1);
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	//다시 저장한다.
	//1. 이전의 내용을 임시화일로 저장 (저장전) -> TMP_FILE2
	fp1=fopen(NOW_FILED,"r+");
	fp2=fopen(TMP_FILE2,"w");
	for (i=0;i<bbs.posi;i++) fputc(fgetc(fp1),fp2);
	if (fp1!=NULL) fclose(fp1);

	//2. 새로 추가된 화일을 저장  (TMP_FILE3) -> TMP_FILE2
	bbs.size=file_size(TMP_FILE3) ; 		 //새로작성한 게시물의 길이
	bbs.line=0;
	if (bbs.size<0) bbs.size=0;
	if (bbs.size>1000000) bbs.size=1000000;
	if ((fp1=fopen(TMP_FILE3,"r"))!=NULL) {
		for (i=0;i<bbs.size;i++) {
			c=fgetc(fp1);
			fputc(c,fp2);
			if (c=='\n') bbs.line++;
		}
		fclose(fp1);
	}

	//3. 나머지화일을 저장 변경된 화일을 저장	-> TMP_FILE2
	fp1=fopen(NOW_FILED,"r");
	if (fp1==NULL) return No;
	fseekend(fp1);
	total=ftell(fp1) ;			//원래화일의 나머지
	start=oldbbs.posi+oldbbs.size;
	fseek(fp1,start,SEEK_SET);
	for (i=start+1;i<=total;i++) fputc(fgetc(fp1),fp2);
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);

	//4. NOW_FILED 저장
	file_copy(TMP_FILE2,NOW_FILED);  //   TMP_FILE2->NOW_FILED
	unlink(TMP_FILE);
	unlink(TMP_FILE2);
	unlink(TMP_FILE3);

	//변경된 제목화일을 다시 저장
	bbs.ok+=1000000;			   //응답횟수 추가
    strcpy(bbs.passwd,"");
	if ((fp1=fopen(NOW_FILE,"r+"))!=NULL) {      //제목화일
		c_lock(1,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
		fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
		if (!feof(fp1)) fwrite(&bbs,sizeof_bbs,1,fp1);	//현재변경된 크기 저장
		if (bbs.size<1) bbs.size=0;
		change_size=bbs.size-oldbbs.size;  //변경된 크기
		for(i=NOW_REC+1;i<NOW_ALLREC;i++) {
			fseek(fp1,i*sizeof_bbs,SEEK_SET);
			fread(&bbs2,sizeof_bbs,1,fp1);
			bbs2.posi+=change_size;
			fseek(fp1,i*sizeof_bbs,SEEK_SET);
			if (!feof(fp1)) fwrite(&bbs2,sizeof_bbs,1,fp1);
		}
		fclose(fp1);
		c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
	}
	return Yes;
