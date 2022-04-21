
/*
wtest(int mode)  //쓰기테스트
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
            sprintf(bbs.title,"테스트S:%2d %s  #:%2d %s",n,now_str(13),cfg.hostno,tty);
            bbs_add(1);
            msleep(3);
        }
    }
    else {
        time(&start_time);
        for (i=1;i<=10;i++) {
            sprintf(buff,"테스트 ID:%-8.8s TIME:%s HOST:%s HOSTNO:%d\n",pf.id,now_str(10),hostname,cfg.hostno);
            put_line(buff,TMP_FILE);
        }
        for (n=1;n<=500;n++) {
            bbs.del=Yes;
            strcpy(bbs.id,pf.id);
            strcpy(bbs.name,pf.name);
            time(&bbs.date);
            sprintf(bbs.title,"테스트F:%2d %s  #:%2d %s",n,now_str(13),cfg.hostno,tty);
            bbs_add(1);
        }
    }
    unlink(TMP_FILE);
    printf("\n ## 총 %d 초가 소요되었습니다.\n",how_old(start_time));
}
*/

