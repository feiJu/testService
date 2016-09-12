@Rem 如果已存在，则先停止，再删除
echo 停止服务
net stop TSServer
echo 删除服务
sc delete TSServer


@Rem 1. 添加为系统服务
echo 添加为系统服务
sc create TSServer binPath= "%~dp0Debug\testService.exe"
@Rem 2. 设置开机自启动
echo 设置开机自启动   
sc config TSServer start= AUTO  
@Rem 3. 启动服务
echo 启动服务                  
net start TSServer

@Rem echo 启动测试程序，如果有连续日志输出则说明服务程序启动正常。
@Rem start "" "%~dp0Debug\client.exe"

pause