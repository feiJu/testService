@Rem ����Ѵ��ڣ�����ֹͣ����ɾ��
echo ֹͣ����
net stop TSServer
echo ɾ������
sc delete TSServer


@Rem 1. ���Ϊϵͳ����
echo ���Ϊϵͳ����
sc create TSServer binPath= "%~dp0Debug\testService.exe"
@Rem 2. ���ÿ���������
echo ���ÿ���������   
sc config TSServer start= AUTO  
@Rem 3. ��������
echo ��������                  
net start TSServer

@Rem echo �������Գ��������������־�����˵�������������������
@Rem start "" "%~dp0Debug\client.exe"

pause