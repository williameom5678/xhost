
/*
wtest(int mode)  //�����׽�Ʈ
{
    char buff[100];
    int i,n;
    time_t start_time;
    time(&start_time);
    unlink(TMP_FILE);
    if (mode==1) {
        for (n=1;n<=10;n++) {
            unlink(TMP_FILE);
            for (i=1;i<=10;i++) {
                sprintf(buff,"%2d. ID:%-8.8s TIME:%s HOST:%s HOSTNO:%d\n",i,pf.id,now_str(10),hostname,cfg.hostno);
                put_line(buff,TMP_FILE);
            }
            strcpy(bbs.id,pf.id);
            strcpy(bbs.name,pf.name);
            time(&bbs.date);
            sprintf(bbs.title,"�׽�ƮS:%2d %s  #:%2d %s",n,now_str(13),cfg.hostno,tty);
            bbs_add(1);
            msleep(3);
        }
    }
    else {
        time(&start_time);
        for (i=1;i<=10;i++) {
            sprintf(buff,"�׽�Ʈ ID:%-8.8s TIME:%s HOST:%s HOSTNO:%d\n",pf.id,now_str(10),hostname,cfg.hostno);
            put_line(buff,TMP_FILE);
        }
        for (n=1;n<=500;n++) {
            bbs.del=Yes;
            strcpy(bbs.id,pf.id);
            strcpy(bbs.name,pf.name);
            time(&bbs.date);
            sprintf(bbs.title,"�׽�ƮF:%2d %s  #:%2d %s",n,now_str(13),cfg.hostno,tty);
            bbs_add(1);
        }
    }
    unlink(TMP_FILE);
    printf("\n ## �� %d �ʰ� �ҿ�Ǿ����ϴ�.\n",how_old(start_time));
}
*/

