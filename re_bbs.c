

    file_add(buff
    fp1=fopen(TMP_FILE2,"w");
	fp2=fopen(TMP_FILE,"r");
    if (fp1==NULL||fp2==NULL) return No;
	if (mode==0) fprintf(fp1,"\n   ###### [�亯");
    else         fprintf(fp1,"�߰�");


	unlink(TMP_FILE1);
	unlink(TMP_FILE2);
	unlink(TMP_FILE3);

	oldbbs=bbs;

	//������ �ӽ�ȭ�Ϸ� ����  ->TMP_FILE3
	if (!is_file(NOW_FILED)) touch_file(NOW_FILED);
	fp1=fopen(NOW_FILED,"r");
	fp2=fopen(TMP_FILE3,"w");
	fseek(fp1,bbs.posi,SEEK_SET);
	for (i=0;i<bbs.size;i++) fputc(fgetc(fp1),fp2);
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);


	//3. ���� �߰��� ����(TMP_FILE)�� ������ ����(TMP_FILE3)�� �߰�
	fp1=fopen(TMP_FILE3,"a+");
	fp2=fopen(TMP_FILE,"r");
	if (mode==0) fprintf(fp1,"\n   ###### [�亯");
	else		 fprintf(fp1,"\n   ###### [�߰�");
	fprintf(fp1,"%d] ID: %s (%s) �Ͻ� %s ######\n",calc_ok(3)+1,pf.id,pf.nick,now_str(3));

	fseekend(fp2);
	n=ftell(fp2) ;				//�����ۼ��� �Խù��� ����
	rewind(fp2);
	for (i=0;i<n;i++) fputc(fgetc(fp2),fp1);
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	//�ٽ� �����Ѵ�.
	//1. ������ ������ �ӽ�ȭ�Ϸ� ���� (������) -> TMP_FILE2
	fp1=fopen(NOW_FILED,"r+");
	fp2=fopen(TMP_FILE2,"w");
	for (i=0;i<bbs.posi;i++) fputc(fgetc(fp1),fp2);
	if (fp1!=NULL) fclose(fp1);

	//2. ���� �߰��� ȭ���� ����  (TMP_FILE3) -> TMP_FILE2
	bbs.size=file_size(TMP_FILE3) ; 		 //�����ۼ��� �Խù��� ����
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

	//3. ������ȭ���� ���� ����� ȭ���� ����	-> TMP_FILE2
	fp1=fopen(NOW_FILED,"r");
	if (fp1==NULL) return No;
	fseekend(fp1);
	total=ftell(fp1) ;			//����ȭ���� ������
	start=oldbbs.posi+oldbbs.size;
	fseek(fp1,start,SEEK_SET);
	for (i=start+1;i<=total;i++) fputc(fgetc(fp1),fp2);
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);

	//4. NOW_FILED ����
	file_copy(TMP_FILE2,NOW_FILED);  //   TMP_FILE2->NOW_FILED
	unlink(TMP_FILE);
	unlink(TMP_FILE2);
	unlink(TMP_FILE3);

	//����� ����ȭ���� �ٽ� ����
	bbs.ok+=1000000;			   //����Ƚ�� �߰�
    strcpy(bbs.passwd,"");
	if ((fp1=fopen(NOW_FILE,"r+"))!=NULL) {      //����ȭ��
		c_lock(1,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
		fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
		if (!feof(fp1)) fwrite(&bbs,sizeof_bbs,1,fp1);	//���纯��� ũ�� ����
		if (bbs.size<1) bbs.size=0;
		change_size=bbs.size-oldbbs.size;  //����� ũ��
		for(i=NOW_REC+1;i<NOW_ALLREC;i++) {
			fseek(fp1,i*sizeof_bbs,SEEK_SET);
			fread(&bbs2,sizeof_bbs,1,fp1);
			bbs2.posi+=change_size;
			fseek(fp1,i*sizeof_bbs,SEEK_SET);
			if (!feof(fp1)) fwrite(&bbs2,sizeof_bbs,1,fp1);
		}
		fclose(fp1);
		c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
	}
	return Yes;
