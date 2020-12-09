

## 准备工作

* 在系统中编译并且安装 nanomsg

  https://blog.csdn.net/dancer__sky/article/details/81585017



## 编译

```bash
mkdir build
cd build
cmake ..
make -j4
```


## 运行

1. 打开命令行窗口1，运行Master

   ```bash
   ./Master
   ```

2. 打开命令行窗口2，运行Slave

      ```bash
   ./Slave
      ```