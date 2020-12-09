
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

#include "jsonlib/json.hpp"

#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>
#include <nanomsg/reqrep.h>
#include <nanomsg/pair.h>
#include <nanomsg/pubsub.h>

using json = nlohmann::json;

bool exit_ = false;

string process_buf(char* buf,int len)
{
    /*
     * 缓存后处理
     */
    string buf_str;
    for(int i=0;i<len;i++){
        buf_str.push_back(buf[i]);
    }
    return buf_str;
}

int main(int argc, char* argv[])
{
    std::vector<std::vector<float>> float_vector = {{0,0,0},{1,1,1},{2,2,2}};

    /*
     * nanomsg 双向通讯模式 (NN_PAIR) 示例
     * Slave 端
     */

    int master_cmd_sock = nn_socket(AF_SP, NN_PAIR);       // master 通过 pipeline 接收slave的数据
    assert(master_cmd_sock >= 0);
    // 在 Slave 端采用 nn_connect 连接
    assert(nn_connect(master_cmd_sock, "tcp://127.0.0.1:6666")>=0);
    usleep(100000);

    /*
     * nanomsg
     */
    for(;!exit_;)
    {

        char *buf ;
        // 从master端接收数据
        int bytes = nn_recv(master_cmd_sock, &buf, NN_MSG, 0);
        if(bytes>0)
        {
            string buf_str = process_buf(buf,bytes);
            cout<<buf_str<<endl;
        }
        nn_freemsg(buf);

        // 构造一个简单的JSON对象
        json j_object;
        j_object["data_field_1"] = 123;
        j_object["data_field_2"] = "data_field_2";
        j_object["data_field_3"] = float_vector;
        // 将 j_object 打包成字符串传回传给 Master
        string rep_string = j_object.dump();
        int sent = nn_send(master_cmd_sock, rep_string.c_str(), rep_string.size(), 0);

        usleep(10000);
    }

    nn_close(master_cmd_sock);


    return 0;
}








