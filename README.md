# NewRepo
此项目是电子科技大学iArt辅修2B2 Hello World项目
开发工具为使用Microsoft Visual Studio 2019
SDK（Software Development Kit软件开发工具包）：AstraSDK-v2.0.18（奥比中光Astra Pro）和OpenCV SDK

部署过程：下载奥比中光摄像头驱动进行安装，下载AstraSDK-v2.0.18和Open CV的SDK，在VS里进行属性配置好，
属性设置里 C/C++ 附加包含目录 和 链接器附加库目录 输入配置astra.lib,astra_core.lib,astra_core_api.lib  
直到所有头文件能够include就行了。

可以开始运行。
