
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
    std::vector<std::vector<float>> float_vector;

    /*
     * nanomsg 双向通讯模式 (NN_PAIR) 示例
     * master 端
     */

    int slave_cmd_sock = nn_socket(AF_SP, NN_PAIR);       // master 通过 pipeline 接收slave的数据
    assert(slave_cmd_sock >= 0);
    // 在 Master 端采用 nn_bind 连接
    assert(nn_bind(slave_cmd_sock, "tcp://127.0.0.1:6666")>=0);
    usleep(100000);

    /*
     * nanomsg
     */
    for(;!exit_;)
    {
        string req_string = "request from master";
        int sent = nn_send(slave_cmd_sock, req_string.c_str(), req_string.size(), 0);

        char *buf ;
        //receive other messages from Slave/AI/drone
        int bytes = nn_recv(slave_cmd_sock, &buf, NN_MSG, 0);
        if(bytes>0)
        {
            string buf_str = process_buf(buf,bytes);
            // 直接显示收到的JSON数据
            cout<<buf_str<<endl;

            // 将收到的JSON数据拆包
            json j_object = json::parse(buf_str.c_str());
            int data_field_1 = j_object["data_field_1"];
            string data_field_2 = j_object["data_field_2"];
            for(int i=0;i<3;i++)
            {
                std::vector<float> sub_vector;
                for(int ii=0;ii<3;ii++)
                {
                    sub_vector.push_back(j_object["data_field_3"][i][ii]);
                }
                float_vector.push_back(sub_vector);
            }   // 拆包完成
        }
        nn_freemsg(buf);

        usleep(10000);
    }

    nn_close(slave_cmd_sock);

    return 0;
}








