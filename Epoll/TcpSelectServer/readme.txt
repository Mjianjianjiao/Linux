1.简单的服务器与客户端进行通信
2.新连接的描述符会被覆盖，需要一个数组保存
3.select返回时会将没有就绪的描述符置0，所以每次都要用保存的描述符进行初始化
