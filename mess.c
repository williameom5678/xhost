////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// �޼��� ���۰���
///////////////////////////////////////////////////////////////////////////
// ���̵��:dark����϶� moni���� dark ������

#include <fcntl.h>
#include <stdarg.h>
#include "bbs.h"
#include "chat.h"


//�޼��� ������ ����
//say  �Ϲ�����
//mm   �޼�������
////   ��ũ������

//�ش���Ʈ 1�� �޼����� ������.
message(const char *fmt,...)
{
	char filename[250];
	char mess[400];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(mess,fmt, ap);
	va_end(ap);
	if (strlen(mess)>0) {
		sprintf(filename,"user/message/%s.mes",no_spc(tty));
		buff2file(mess,filename);
	}
}

//���� �޴��� ��ü �̿��ڿ��� �ӼӸ� ���ϱ�
//mode==0:��üȸ�� 1:���絿ȣȸ ��ü 2:�մ���ü 3:ȸ����ü
//		4:���� �޴� ��üȸ������ 8.�ٷ� ���� 9:������ȸ������
to_all_say(int mode,int mode2,char *buffstr)
{
	FILE *fp;
	char toid[9];
	char messbuff[300],buff[300],filename[100];
	MONI amoni[1500];
	int first;
	int i,n,count,found;

	strcpy(toid,"");
		 if (pf.type<1||pf.type<cfg.allsay1) {view("frm/say/notall1.mes");return;}
	else if (mode==0&&pf.type<cfg.allsay2) {view("frm/say/notall2.mes");return;}
	else if (mode==1&&pf.type<cfg.allsay3) {view("frm/say/notall3.mes");return;}

	check_cmd(buffstr);
	for (i=0;i<1500;i++) strcpy(amoni[i].id,"");
	//���̵� �����ȵ����� �����ؼ� ���� ���ɿ��θ� Ȯ���ؼ� moni�� ����
	count=0;
	for (n=0;n<=cfg.count_server;n++) {
		sprintf(filename,"user/moni/moni%d.dat",n);
		if ((fp=fopen(filename,"r"))==NULL) continue;
		while (count<1500&&fread(&moni,sizeof_moni,1,fp)) {
			if (strlen(moni.tty)==0||strlen(moni.id)<3||moni.mode==0) continue;
			if ((moni.mode==5||moni.mode==6||moni.mode==8||
				  moni.mode==14||moni.mode==16||moni.mode==18||moni.mode==19)) continue;
			if ( (mode==4&&is_same(moni.gocode,go.code)&&is_same(moni.club,club_go.code))||
				 (mode==1&&is_same(moni.club)&&club_go.code)||
				 (mode==2&&moni.type==0)||
				 (mode==3&&moni.type>0)||
				 (mode==8)||
				 (mode==0) ) {
				found=No;
				for (i=0;i<count;i++) {
					if (amoni[i].hostno==moni.hostno&&is_same(amoni[i].tty,moni.tty)) {
						found=Yes;
						break;
					}
				}
				if (!found) amoni[count++]=moni;
			}
		}
		fclose(fp);
	}

	_cont0:
	if (count<1) {printf("\n ## ���۴�����\n");goto _endsay;}

	first=Yes;
	printf("���۴��: ");
	for (i=0;i<count;i++) {
		printf("%-8s ",amoni[i].id);
		if (i%6==5) printf("          ");
	}
	if (i%6!=0) printf("\n");
	view("frm/say/allsay.mes");
	printf("��ü�޼���>",buff);
	lineinput(buff,250);check_han(buff);printf("\n");
	if (strlen(buff)<3) goto _endsay;
	if	(bit(cfg.mode,16)) sprintf(messbuff," %s[7m%-8s[m>>\"%s\"\n",first?"\n":"",pf.id,buff);
	else				   sprintf(messbuff," %s%-8s>>\"%s\"\n",first?"\n":"",pf.id,buff);

	//���� �޼��� ����
	for (i=0;i<count;i++) {
		if (amoni[i].mode==3) { 	//��ȭ��
			del_enter(messbuff);
			sprintf(buff,"7[21;1H\n[21;1H%s[23;1H8",messbuff);
			to_tty(amoni[i].hostno,amoni[i].tty,buff);		//��ȭ���϶�
		}
		else to_tty(amoni[i].hostno,amoni[i].tty,messbuff);
	}
	_endsay:
	view("frm/say/allsay.end");
	return;
}



//mode==2 : ��ȭ�ǿ���.. ����	mode==3 ������ ����(�û�������)
//Ư���̿��ڿ��� �ӼӸ� ���ϱ�

to_say(int mode,char *idstr,char *buffmess)
{
	char outmess[300],buff[300],buffstr[300];
	char toid[9];
	char macro=No;

	strcpy(outmess,"");
	strcpy(buffstr,buffmess);
	strcpy(toid,idstr);
	strcpy(g_id,toid);
	strcpy(g_name,toid);
	if (guest_mode&&bit(cfg.chatmode,10)) {
		view("frm/error/noguest.err");
		goto _fail;
	}
	if (mode!=0) re_print = Yes;
	check_cmd(buffstr);
	if (is_same(mmc[0],"to")) strcpy(mmc[0],"say");
	if (is_same(mmc[0],"/ear")) strcpy(mmc[0],"/say");
	if (is_same(mmc[0],"/mm")) strcpy(mmc[0],"/to");
	if (mmc[0][0]=='/') {
		if (!is_same(mmc[0],"/say")&&
			!is_same(mmc[0],"/in")&&
			!is_same(mmc[0],"/to")
			) macro=Yes;  //��ũ��
	}
	if (mode==3&&(pf.type<18&&!in_file2("system/say2.id",pf.id))) mode=0;
	if (strlen(toid)<1) strcpy(toid,mmc[1]);
	strcpy(g_id,toid);
	if (strlen(g_name)<1) strcpy(g_name,moni.name);
	if (strlen(g_name)<1) strcpy(g_name,g_id);
	pf.todaymode=set_bit(pf.todaymode,3,1);   //�����۾�

	//���̵� �����ȵ����� �����ؼ� ���� ���ɿ��θ� Ȯ���ؼ� moni�� ����
	if (strlen(toid)<1) {
		if (mode==2) printf("[21;1H\n");  //��ȭ�濡�� ����
		if (bit(cfg.messmode,8)) sel_user2();
		else sel_user("frm/say/say2.mes");
		strcpy(toid,moni.id);
		strcpy(g_id,moni.id);
		strcpy(g_name,moni.name);
	}
	if (strlen(toid)<3||!is_moni(toid)) goto _fail; 	//�������� �ƴҶ�
	strcpy(g_id,toid);
	if (read_pf4(g_id)) strcpy(g_name,pf2.name);
	if (strlen(g_name)<1) strcpy(g_name,moni.name);
	if (strlen(g_name)<1) strcpy(g_name,g_id);
	buff2cmd(buffstr,outmess,2);	 //�޼����� ���� �Է��� (�޼��� ����)
	if (macro) {
		bzero(outmess,300);
		if (toid[0]==0) strcpy(toid,"ȸ��");
		if (!say_macro(outmess,buffstr,pf.id,toid)) {
			if (mode==2) printf("[21;1H\n");  //��ȭ�濡�� ����
			printf(" %s ��ũ�� ����� �����ϴ�.\n",mmc[0]);
			return;
		}
	}
	if (bit(pf.mymode,9)) view("frm/say/msgon.log");

	//�ڷ������ΰ�?  (SAY2�϶��� �˻繫��)
	to_say_tty(mode,toid,outmess,macro);
	sprintf(buff,"tmp/tmp7%d%s",cfg.hostno,no_spc(tty));
	fput_str(toid,buff,10);
	return;

	_fail:
	if (strlen(toid)<3) return;
	else if (read_pf4(toid)) {
		view("frm/say/notlog.mes");
		if (yesno2("",Yes)) to_memo(mode,toid,"");
		printf("\n");
	}
	else {
		view("frm/say/notid.mes");
	}
	return;

	_notsay:
	if (strlen(toid)<3) return;
	view("frm/say/notsay.mes");
	if (yesno2("",Yes)) {
		to_memo(mode,toid,outmess);
	}
	printf("\n");
	return;
}


//mode==2 : ��ȭ�ǿ���.. ����	mode==3 ������ ����(�û�������)
to_say3(int mode,char *idstr,char *buffmess)
{
	char macro=No;
	strcpy(g_id,idstr);
	strcpy(g_name,idstr);
	if (guest_mode&&bit(cfg.chatmode,10)) return;
	if (!is_moni(idstr)) return;	 //�������� �ƴҶ�
	if (read_pf4(g_id)) strcpy(g_name,pf2.name);
	if (strlen(g_name)<1) strcpy(g_name,moni.name);
	if (strlen(g_name)<1) strcpy(g_name,g_id);
	to_say_tty(mode,idstr,buffmess,macro);
}



//mode==2 : ��ȭ�ǿ���.. ����	mode==3 ������ ����(�û�������)
//Ư���̿��ڿ��� �ӼӸ� ���ϱ�
to_say_tty(int mode,char *toid,char *outmess,int macro)
{
	char messbuff[300],buff[300];
	char buff1[200];
	int first,say_count;
	PF old_pf2;
	old_pf2=pf2;

	first=Yes;
	say_count=0;

	strcpy(g_id,toid);
	if (read_pf4(g_id)) strcpy(g_name,pf2.name);
	if (strlen(g_name)<1) strcpy(g_name,toid);
	if (outmess[0]==0&&!macro) {
		view("userdata/%s/%s/pfinfo.say",c_han(toid),toid);
		view("frm/say/saymes.mes");
	}
	while (1) {
		say_count++;
//		  printf("***%d %d %d***",cfg.sayline,say_count,macro);
		if (cfg.sayline>0&&say_count>cfg.sayline) goto _endsay;
		if (cfg.sayline==255) goto _endsay;
		if (outmess[0]==0) {
			if (mode==2) printf("[23;1H");
			sprintf(buff," [%s]",toid);
			printf(" %-10s>",buff);
			lineinput(buff,250);check_han(buff);printf("\n");
			if (buff[0]=='\\'&&buff[1]=='/') {buff[0]=' ';all_trim(buff);}
			if (is_same(buff,"//us")) {view_user(1,1,"");continue;}
			if (strlen(buff)>3&&buff[0]=='/'&&buff[1]=='/') {
				strcpy(buff1,&buff[2]);
				strcpy(auto_cmd,buff1);
				printf("\n\n\n");
				return;
			}
		}
		else {
			strcpy(buff,outmess);
		}
		if (strlen(buff)<1||is_same(buff,".")||
			is_same(buff,"q")||is_same(buff,"p")||is_same(buff,"/q")) break;
		if (is_same(buff,"/st")||is_same(buff,"/us")) {
			view_user(1,1,"");
			say_count--;
			continue;
		}
		if	(bit(cfg.mode,16)) sprintf(messbuff," {[7m%-8s[m}\"%s\"\n",pf.id,buff);
		else				   sprintf(messbuff," {%-8s}\"%s\"\n",pf.id,buff);

		//����ȸ���� ID,TTY�� �̾Ƽ� moni�� ����
		if (first) { //ó�������� \n ����
			sprintf(buff,"%c\n",bit(pf2.mymode,23)?"\007":"");
			if (!to_moni(toid,mode,buff)) goto _failsay;
			first=No;
		}
		if (is_char(messbuff,'$')) strchng2(messbuff,"$#","$ #");
		if (!to_moni(toid,mode,messbuff)) goto _failsay;
		if (macro||strlen(outmess)>0) break;
	}

	_endsay:
	strcpy(g_id,toid);
	if (read_pf4(g_id)) strcpy(g_name,pf2.name);
	else				strcpy(g_name,g_id);
	if (say_count>1) view("frm/say/sayend.mes");
	if (mode==2) printf("[21;1H\n");
	if (mode!=2&&macro) printf("\n %s\n",outmess);
	if (mode!=2) printf("\n\n\n");
	pf2=old_pf2;
	return;

	_failsay:
	if (mode==2) printf("[s[21;1H\n");
	view("frm/say/sayfail.mes");
	if (mode==2) printf("[23;3H[u");
	pf2=old_pf2;
	return;
}

//Ư���̿��ڿ��� ���� ���ϱ�
to_memo(int mode,char *idstr,char *buffstr)
{
	FILE *fp;
	char toid[20];
	char buff[700];
	char filename[300];

	char messbuff[700];
	char buff3[101][300];
	int i,count;

	strcpy(messbuff,buffstr);  //�޼��� �м�
	for (i=0;i<100;i++) strcpy(buff3[i],"");
	if (guest_mode&&bit(cfg.chatmode,11)) {
		view("frm/say/noguest.mes");
		return No;
	}
	if (strlen(idstr)>3) strcpy(mmc[1],idstr);
	if (strlen(mmc[1])<3) {  //���̵� �����ȵ�
		if (mode==2) printf("[21;1H\n");  //��ȭ�濡�� ����
		if (bit(cfg.messmode,8)) sel_user2();
		else sel_user("frm/say/memo1.mes");
		strcpy(mmc[1],moni.id);
		strcpy(toid,moni.id);
	}
	strcpy(toid,mmc[1]);
	if (strlen(toid)<3) goto _fail;
	if (!read_pf4(toid)) {
		printf("\n ### [%s] ȸ���� ã���� �����ϴ�.",toid);
		goto _fail;
	}
	if (strcmp(toid,pf2.id)!=0) {
		if (check_sameid(toid)) {
			mess("%s ���� %s ID�� ����Ǿ����ϴ�",toid,g_buff2);
			strcpy(toid,g_buff2);
			if (!read_pf4(toid)) goto _fail;
		}
	}
	strcpy(g_id,pf2.id);
	strcpy(g_name,pf2.name);

	count=0;
	//�ش�Ǵ� ���̵� ���� �����
	if (strlen(messbuff)==0) view("userdata/%s/%s/pfinfo.say",c_han(toid),toid);
	if (strlen(messbuff)>0) {
		strcpy(buff3[0],messbuff);
		count=1;
	}
	else {
		view("frm/say/tomess5.mes");
		count=0;
		for (i=0;i<99;i++) {
			printf("\n%2d.",i+1);lineinput(buff,290);check_han(buff);
			if (is_same(buff,"q")||is_same(buff,"p")||is_same(buff,"/q")) {
				view("frm/say/tomess6.mes");
				return No;
			}
			if (buff[0]==0||is_same(buff,".")) break;
			strcpy(buff3[i],buff);
			count++;
		}
	}
	if (count<1) goto _fail;
	sprintf(filename,"userdata/%s/%s/mess.dat",c_han(toid),toid);
	if ((fp=fopen(filename,"a+"))==NULL) if ((fp=fopen(filename,"w+"))==NULL) return No;
	fprintf(fp,"  [7m �� %s(%s)���� ���� �޼��� ��   %s [0;1m\n",pf.id,pf.name,now_str(39));     //�޼��� ���ۿ� ����
	for (i=0;i<count;i++) {
		del_esc(buff3[i]);
		if (strlen(buff3[i])>0) {
			fprintf(fp,"%s\n",buff3[i]);     //�޼��� ���ۿ� ����
		}
	}
	if (count>1) fprintf(fp," ........ �޼��� �� .......\n");     //�޼��� ���ۿ� ����
	fclose(fp);
	if (count>1) {
		sprintf(buff," ##%s ���� �޼����� �����߽��ϴ�.%s",pf.id,bit(pf2.mymode,23)?"\007":"");
		to_mess(pf2.id,buff);
	}
	printf("\n ### %s(%s)�Բ� �޼����� ������ϴ�.\n",pf2.id,pf2.name);
	return Yes;
	_fail:
	printf("\n ### ��ҵǾ����ϴ�.\n");
	return No;
}


//�ش���̵� ����ڿ��� �޼����� ������.
to_mess(char *id,char *mess)
{
	char buff[1000];
	//id�˻�->moni;
	if (!is_moni(id)) return No;
	if (moni.mode==3) sprintf(buff,"7[21;1H\n[21;1H%s[23;1H8",mess);
	else			  strcpy(buff,mess);
	to_tty(moni.hostno,moni.tty,buff);		//��ȭ���϶�
	return Yes;
}


//�ش���̵� ����ڿ��� �޼����� ������.
to_moni(char *tid,int mode,char *messbuff)
{
	char outbuff[5000];
	char buff[5000];
	//�ʴ��� ������� tty�� ã�Ƴ�
	strcpy(outbuff,messbuff);
	if (strlen(messbuff)>1&&messbuff[strlen(messbuff)-1]!='\n') strcat(outbuff,"\n");
	if (is_moni(tid)==0)	 return No; //id�˻�->moni;
	if (is_id_up(mode,moni)) return No;
	if (moni.mode==3) { 	//��ȭ��
		del_enter(outbuff);
		sprintf(buff,"7[21;1H\n[21;1H%s[23;1H8",outbuff);
		to_tty(moni.hostno,moni.tty,buff);		//��ȭ���϶�
	}
	else to_tty(moni.hostno,moni.tty,outbuff);
	sprintf(buff,"tmp/tmp7%d%s",moni.hostno,no_spc(moni.tty));
	fput_str(pf.id,buff,10);
	return Yes;
}

//�ش���̵� ����ڿ��� �޼����� ������.
to_moni2(MONI moni,int mode,char *messbuff)
{
	char outbuff[5000];
	char buff[5000];
	//�ʴ��� ������� tty�� ã�Ƴ�
	strcpy(outbuff,messbuff);
	if (is_id_up(mode,moni)) return No;
	if (moni.mode==3) { 	//��ȭ��
		del_enter(outbuff);
		sprintf(buff,"7[21;1H\n[21;1H%s[23;1H8",outbuff);
		to_tty(moni.hostno,moni.tty,buff);		//��ȭ���϶�
	}
	else to_tty(moni.hostno,moni.tty,outbuff);
	sprintf(buff,"tmp/tmp7%d%s",moni.hostno,no_spc(moni.tty));
	fput_str(pf.id,buff,10);
	return Yes;
}


//�ش���̵� ���Ǵ� �޼��� ���� ������ �ľ��Ѵ�.
is_id_up(int mode,MONI moni)
{
	FILE *fp;
	int n,i,rec,found=No;
	char filename[100];
	if(moni.mode!=0&&(mode==3||moni.mode!=99)) {
		found=Yes;
	}
	if (!found) {
		view("frm/say/notlog.mes");
	}
	else if (moni.mode==4||
		(moni.mode==5&&!bit(cfg.messmode,4))||
		moni.mode==6||
		moni.mode==8||
		moni.mode==14||
		(moni.mode==3&&bit(cfg.messmode,5))||
		(moni.mode==4&&bit(cfg.messmode,6))||
		(moni.mode==15&&bit(cfg.messmode,6))||
		(moni.mode==16&&bit(cfg.messmode,1))||
		moni.mode==19) {
		view("frm/say/notsay%d.mes",moni.mode);
		return Yes;
	}
	else if (moni.msg)		 {
		view("frm/say/notmsg.mes");
		return Yes;
	}
	return No;
}


say_macro(char *messbuff,char *cmd2,char *id1,char *id2)
{
	FILE *fp;
	char buff[1500];
	char buff1[1500];
	char buff2[1500];
	char cmd[100];
	int ret=No;
sscanf(cmd2,"%s",cmd);
	strcpy(messbuff,"");
	if (strlen(cmd)<2||cmd[0]!='/') return No;
	strchng2(cmd,"/","");strchng2(cmd,"/","");

	//��ũ�� ȭ���б�
	if ((fp=fopen("frm/say/macro.txt","r"))==NULL) {
		printf("\n frm/say/macro.txt ��ũ�� ������ �����ϴ�.");
		return ret;
	}
	while (fgets(buff,200,fp)!=NULL) {
		del_enter(buff);
		if (buff[0]!=0&&!is_char("#;",buff[0])&&strlen(buff)>6) {
			buff2buff1(buff,buff1,buff2);
			if (is_same(buff1,cmd)) {
				del_enter(buff2);
				strcpy(messbuff,buff2);
				strchng2(messbuff,"!me!",id1);
				strchng2(messbuff,"!to!",id2);
				ret=Yes;
				break;
			}
		}
	}
	fclose(fp);
	return ret;
}



//mode=0 �Ϲ�(mess)  1:��ȭ�� (printf)
set_msg(int mode,char *buffstr) 	 //�޼���,�ʴ� ���� ����
{
	char buff[100];
	strcpy(buff,buffstr);del_esc(buff);
	buff[1]=0;
	if		(is_same(buff,"y")) pf.mymode=set_bit(pf.mymode,9,0);
	else if (is_same(buff,"n")) pf.mymode=set_bit(pf.mymode,9,1);
	else if (buff[0]==0)		pf.mymode=gtt_bit(pf.mymode,9);
	if (bit(pf.mymode,9)) printf("\n## �ʴ�(�޼���) �Ұ� ���·� ���� (MSG N) **\n");
	else				  printf("\n## �ʴ�(�޼���) ���� ���·� ���� (MSG Y) **\n");
	if (!guest_mode&&pf.type>0) save_pf(34);
}




to_tty(int hostno,char *stty,char *outmess)
{
	char buff[100];
	char bufftty[100];
	int fout;
	strcpy(bufftty,stty);
	strchng2(bufftty,"/dev/","");
	if (is_same(bufftty,"www")) {
		mess("WWW �����ڷ� �޼����� �����Ҽ� �����ϴ�.");
		return No;
	}
//	  printf("�޴»��: HNO:%d  TTY:%s ��:'%s'\n",hostno,stty,outmess);
//	  printf("��������: HNO:%d  TTY:%s ID:'%s'\n",cfg.hostno,tty,pf.id);
	if (strlen(bufftty)<1) return No;
	if (cfg.count_server==1) hostno=cfg.hostno;
	if (cfg.hostno != hostno) {
		return (svr_send(hostno,stty,outmess));   //�ٸ� ������ ����
	}
	sprintf(buff,"/dev/%s",bufftty);
	del_space(buff);
	fout = open(buff,O_WRONLY|O_NDELAY);
	if (fout==-1) {
		printf("\n\n ## **** %s ERROR. (�޼������۽��� H:%d)****\n\n\n\n",buff,hostno);
		return No;
	}
	else {
		write(fout,outmess,strlen(outmess));
		close(fout);
	}
	return Yes;
}


to_tty2(int hostno,char *tty,char *outmess)
{
	char buff[100];
	int fout;
	del_space(tty);
	if (is_same(tty,"www")) return No;
	if (strlen(tty)<1) return No;
	if (cfg.count_server<=1) hostno=cfg.hostno;
	if (bit(cfg.messmode,7)||cfg.hostno != hostno)
		return (svr_send(hostno,tty,outmess));
	sprintf(buff,"/dev/%s",tty);
	fout = open(buff,O_WRONLY|O_NDELAY);
	if (fout != -1) {
		write(fout,outmess,strlen(outmess));
		close(fout);
		return Yes;
	}
	return No;
}

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

svr_send(int hostno, char *stty,char *message)
{
	char send_message[1010],bufftty[100],buff[200],buff1[200],buff2[200];	// ���ۿ� ����
	int i,ii,sockfd ;					// ���� ��ȣ
	unsigned short int server_port=SERVER_PORT[0];
	struct sockaddr_in address; 	// ���Ͽ� ����� �ּ� ����ü
	int result=0, count;			// Client�� ���� ���� ���� ǥ��
	char mess_file[128];
	int nwrite = -1;
	FILE *fp;

	strcpy(bufftty,stty);
	strchng2(bufftty,"/dev/","");
	if (cfg.count_server<=1) hostno=cfg.hostno;

	bzero(send_message,1010);
	if (strlen(message)>1000) message[1000]=0;
    sprintf(send_message,"\255\254\255M%-10s%s",bufftty,message);  // ���ۿ� ���ۿ� ���� ����
    sockfd=socket(AF_INET,SOCK_STREAM, 0) ;    // ���� ����
	address.sin_family = AF_INET ;		// ARPA Internet Protocol ���
	sprintf(buff,"%s.%u",cfg.sip,cfg.nip[hostno%100]);  // �޴� ���� IP
	address.sin_addr.s_addr = inet_addr(buff);

    for (ii=0;ii<5;ii++) {
		count=0;
		result=0;
		while (count<5) {
			server_port=(unsigned short int)SERVER_PORT[count++];
			address.sin_port = htons(server_port) ; 	 // Port ����
			result=connect(sockfd,(struct sockaddr *)&address,sizeof(address));
            // Client�� ���� ��û
			if(result == -1) {
			usleep(10000); continue;}
			result=write(sockfd,send_message,sizeof(send_message));
            // ���ӿ� ���� �ߴٸ� send_message�� ���� ����
            if(result == -1) {msleep(1);continue;}
			break;
		}
		if (result != -1) break;
        msleep(3);
	}
	close(sockfd);	   // ���� �Ҹ�
    if (result==-1) return No;
    return Yes;    // �Ϸ�
}
