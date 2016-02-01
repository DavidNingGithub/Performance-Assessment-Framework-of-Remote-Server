:run.bat
//start Debug\Client.exe "echo,127.0.0.1,8088,16,filename,test test test" "8088" "3" "2"
start Debug\Client.exe "echo,127.0.0.1,8086,16,filename,test test test" "8086" "2" "1"
//start Debug\Client.exe "echo,127.0.0.1,8082,16,filename,test test test" "8082" "1" "0"
Debug\Sever.exe
:pause